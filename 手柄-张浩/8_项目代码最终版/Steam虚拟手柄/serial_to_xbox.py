# -*- coding: utf-8 -*-
"""
串口手柄 -> 虚拟 Xbox 360 手柄 桥接脚本（方案二）

原理：读取 STM32 手柄的串口数据（"x:y:x:y:按键:按键"），喂给 ViGEmBus 创建的
      “虚拟 Xbox 360 手柄”。Steam 会把它识别成真正的 Xbox 手柄：
        - 摇杆是真正的模拟量（有力度/线性）
        - 可以用 Steam Input 随意改键

依赖安装：
    1. 下载并安装 ViGEmBus 驱动（一次即可，免费开源）：
       https://github.com/ViGEm/ViGEmBus/releases
    2. 安装依赖（命令行执行）：
       pip install vgamepad pyserial

使用步骤：
    1. 改下面 PORT 为你的手柄串口（如 COM11）
    2. 插好手柄，先运行本脚本
    3. 打开 Steam 游戏，游戏里会显示“Xbox 360 Controller”
    4. 退出按 Ctrl+C

注意：如果某根摇杆方向反了（上下/左右颠倒），把对应 norm() 前面加个负号即可。
"""

import time
import serial
import serial.tools.list_ports
import vgamepad as vg

# ====================== 可改配置 ======================
PORT = "COM1"        # 手柄串口（设备管理器里 CH340 的 COM 号）
BAUD = 9600
CENTER = 1.7          # 摇杆中立电压 1.7V
DEAD = 0.1            # 摇杆死区 0.1V
RELEASE_TIMEOUT = 0.2 # 超过这个时间没收到数据，就把摇杆/按键复位（秒）
# ======================================================

# 左按键模块（5 键）电压 -> Xbox 按钮
LEFT_KEYS = {
    0.0:  vg.XUSB_BUTTON.XUSB_GAMEPAD_A,               # 0V    -> A
    0.47: vg.XUSB_BUTTON.XUSB_GAMEPAD_B,               # 0.47V -> B
    1.0:  vg.XUSB_BUTTON.XUSB_GAMEPAD_X,               # 1.0V  -> X
    1.64: vg.XUSB_BUTTON.XUSB_GAMEPAD_Y,               # 1.64V -> Y
    2.4:  vg.XUSB_BUTTON.XUSB_GAMEPAD_LEFT_SHOULDER,   # 2.4V  -> LB
}

# 右按键模块（5 键）电压 -> Xbox 按钮
RIGHT_KEYS = {
    0.0:  vg.XUSB_BUTTON.XUSB_GAMEPAD_RIGHT_SHOULDER,  # 0V    -> RB
    0.47: vg.XUSB_BUTTON.XUSB_GAMEPAD_START,           # 0.47V -> Start
    1.0:  vg.XUSB_BUTTON.XUSB_GAMEPAD_BACK,            # 1.0V  -> Back
    1.64: vg.XUSB_BUTTON.XUSB_GAMEPAD_LEFT_THUMB,      # 1.64V -> 按左摇杆 L3
    2.4:  vg.XUSB_BUTTON.XUSB_GAMEPAD_RIGHT_THUMB,     # 2.4V  -> 按右摇杆 R3
}


def nearest_key(voltage, table):
    """把电压匹配到最近的按键；未按下(约3.3V)返回 None。"""
    if voltage >= 2.9:
        return None
    best, best_d = None, 0.35
    for v, btn in table.items():
        d = abs(voltage - v)
        if d < best_d:
            best_d, best = d, btn
    return best


def norm(v):
    """电压 -> -1.0..1.0。中心 1.7V，满偏约 1.6V。"""
    if abs(v - CENTER) < DEAD:
        return 0.0
    return max(-1.0, min(1.0, (v - CENTER) / 1.6))


def main():
    ports = [p.device for p in serial.tools.list_ports.comports()]
    print("检测到串口:", ports if ports else "（无）")

    gamepad = vg.VX360Gamepad()
    print("虚拟 Xbox 360 手柄已创建。")

    try:
        ser = serial.Serial(PORT, BAUD, timeout=0.1)
    except Exception as e:
        print("打开串口失败:", e)
        print("请确认：1) PORT 串口号正确  2) 串口助手没占用这个口")
        input("按回车退出...")
        return

    print("已连接", PORT, "。打开 Steam 游戏即可，Ctrl+C 退出。")
    print("提示：现在摇一摇摇杆 / 按一下按键。若下面一直不出现“收到手柄数据”，")
    print("      说明", PORT, "不是手柄那个口，换个 COM 号改 PORT 再试。")

    buf = b""
    held = set()            # 当前按住的 Xbox 按钮
    last_data = time.time()
    got_data = False        # 是否收到过手柄数据
    last_print = 0.0

    try:
        while True:
            n = ser.in_waiting
            if n:
                buf += ser.read(n)
                while b"\n" in buf:
                    line, buf = buf.split(b"\n", 1)
                    text = line.decode("ascii", "ignore").strip("\r\n ")
                    parts = text.split(":")
                    if len(parts) != 6:
                        continue
                    try:
                        x1, y1, x2, y2, kL, kR = [float(p) for p in parts]
                    except ValueError:
                        continue

                    last_data = time.time()

                    now = time.time()
                    if not got_data:
                        got_data = True
                        last_print = now
                        print(">>> 收到手柄数据:", text)
                    elif now - last_print > 2:
                        last_print = now
                        print(">>> 数据正常:", text)

                    # ---- 摇杆 -> 左右拇指摇杆（-1..1）----
                    # 左摇杆控制移动，右摇杆控制视角
                    # 注意：Y 轴这里取了负号，用来修正“上推却往下”的反向问题
                    gamepad.left_joystick_float(norm(x1), -norm(y1))
                    gamepad.right_joystick_float(norm(x2), -norm(y2))

                    # ---- 按键 -> Xbox 按钮 ----
                    desired = set()
                    b = nearest_key(kL, LEFT_KEYS)
                    if b:
                        desired.add(b)
                    b = nearest_key(kR, RIGHT_KEYS)
                    if b:
                        desired.add(b)

                    for btn in list(held - desired):
                        gamepad.release_button(button=btn)
                    for btn in desired - held:
                        gamepad.press_button(button=btn)
                    held = desired

                    gamepad.update()

            # ---- 松手复位：下位机只在“偏离”时才发数据，不发了就当松手 ----
            if time.time() - last_data > RELEASE_TIMEOUT:
                if held:
                    for btn in held:
                        gamepad.release_button(button=btn)
                    held.clear()
                gamepad.left_joystick_float(0.0, 0.0)
                gamepad.right_joystick_float(0.0, 0.0)
                gamepad.update()
                last_data = time.time()

            time.sleep(0.005)

    except KeyboardInterrupt:
        print("\n已退出")
    finally:
        try:
            ser.close()
        except Exception:
            pass


if __name__ == "__main__":
    main()
