# -*- coding: utf-8 -*-
"""
游戏手柄 -> 电脑 桥接程序（不用改游戏源码、不用装 Qt）

作用：直接读手柄的串口数据，把「摇杆」转成鼠标移动（控制飞机），
      把「按键」转成键盘按键（释放技能），从而控制 PlaneWar 游戏。

原理：PlaneWar 本来就支持鼠标移动飞机（mouseMoveEvent）和键盘放技能
      （keyPressEvent），所以用它自己的鼠标/键盘通道就绕开了游戏里
      那段有 bug 的串口代码（开错口 + readAll 丢帧）。

======== 使用步骤 ========
1. 安装 Python 3（如果还没有）
2. 安装依赖（在命令行执行）：
       pip install pyserial pynput
3. 改下面 PORT 为你的手柄串口（如 COM11；不知道就填 "AUTO" 自动扫描）
4. 先运行本脚本，再打开游戏，用鼠标点进游戏画面
5. 把鼠标指针移到游戏窗口内，然后摇摇杆 / 按按键即可
6. 退出：在本脚本窗口按 Ctrl+C
===========================
"""

import time
import serial
import serial.tools.list_ports

try:
    from pynput.mouse import Controller as Mouse
    from pynput.keyboard import Controller as Keyboard, Key
except ImportError:
    Mouse = Keyboard = Key = None

# ==================== 可改配置 ====================
PORT = "COM11"      # 手柄串口；填 "AUTO" 自动扫描
BAUD = 9600
CENTER = 1.7        # 摇杆中立电压 1.7V
DEAD = 0.1          # 死区 0.1V，低于它不动作
STEP = 12           # 每帧鼠标移动像素（越大飞机越快）
# ==================================================


def auto_find_port():
    """扫描所有串口，返回第一个能收到 ':' 分隔数据的手柄口。"""
    ports = [p.device for p in serial.tools.list_ports.comports()]
    print("扫描串口:", ports)
    for p in ports:
        try:
            s = serial.Serial(p, BAUD, timeout=0.5)
            time.sleep(0.8)
            if s.in_waiting:
                data = s.read(s.in_waiting)
                if b":" in data:
                    s.close()
                    return p
            s.close()
        except Exception:
            continue
    return None


def key_for(v, right):
    """电压 -> 键盘按键。与游戏一致：
       <0.3V=Q   0.47V=W   1.0V=E   1.64V=R   2.4V=ESC(左)/空格(右)   3.3V=未按"""
    if v >= 2.9:
        return None
    if v < 0.3:
        return "q"
    if v < 0.75:
        return "w"
    if v < 1.3:
        return "e"
    if v < 1.9:
        return "r"
    if v < 2.7:
        return Key.space if right else Key.esc
    return None


def main():
    if Mouse is None or Keyboard is None:
        print("缺少 pynput，请先安装：  pip install pyserial pynput")
        return

    mouse = Mouse()
    kb = Keyboard()

    port = PORT
    if port.strip().upper() in ("AUTO", "自动"):
        port = auto_find_port()
        if not port:
            print("自动扫描没找到手柄串口，请手动改脚本里的 PORT 再运行")
            return
    print("使用串口:", port)

    try:
        ser = serial.Serial(port, BAUD, timeout=1)
    except Exception as e:
        print("打开串口失败:", e)
        print("请确认：1) 串口号正确  2) 没有其它程序(串口助手)占用这个口")
        return

    buf = b""
    pressed = set()          # 记录按下的键，做“按下只触发一次”
    got_data = False
    print("已连接，等待手柄数据...（请把鼠标移到游戏窗口内）Ctrl+C 退出")

    try:
        while True:
            buf += ser.read(ser.in_waiting or 1)

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

                if not got_data:
                    got_data = True
                    print("收到手柄数据：", text)

                # ---- 摇杆 -> 鼠标移动 ----
                dx = dy = 0
                if x1 - CENTER > DEAD:
                    dx = STEP
                elif CENTER - x1 > DEAD:
                    dx = -STEP
                if y1 - CENTER > DEAD:
                    dy = -STEP      # 上推 = 鼠标上移
                elif CENTER - y1 > DEAD:
                    dy = STEP
                # 右摇杆也叠加（双摇杆一起动更快）
                if x2 - CENTER > DEAD:
                    dx += STEP
                elif CENTER - x2 > DEAD:
                    dx -= STEP
                if y2 - CENTER > DEAD:
                    dy -= STEP
                elif CENTER - y2 > DEAD:
                    dy += STEP

                if dx or dy:
                    mouse.move(dx, dy)

                # ---- 按键 -> 键盘（按下只触发一次，松开才允许再次触发）----
                for idx, (v, right) in enumerate(((kL, False), (kR, True))):
                    k = key_for(v, right)
                    tag = "L" if not right else "R"
                    if k and tag not in pressed:
                        pressed.add(tag)
                        kb.press(k)
                        kb.release(k)
                        print("按键", tag, "->", k)
                    if not k:
                        pressed.discard(tag)

    except KeyboardInterrupt:
        print("\n已退出")
    finally:
        try:
            ser.close()
        except Exception:
            pass


if __name__ == "__main__":
    main()
