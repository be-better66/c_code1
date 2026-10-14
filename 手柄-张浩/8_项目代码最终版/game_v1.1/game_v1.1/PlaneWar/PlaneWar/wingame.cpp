#include "wingame.h"
#include <QVBoxLayout>
WinGame::WinGame(QWidget *parent) :
    QWidget(parent)
{
}

WinGame::WinGame(int x){
    Return = new QPushButton("Return");
//    Next = new QPushButton("Next");
    Score = new QLabel(this);
    Score->setText(QString(SCORE).arg(x));
    Win = new QLabel(this);
    Win->setText("YOU WIN");
//    Next->setFont(QFont("Algerian",18));
//    Next->setStyleSheet("QPushButton{background: transparent; color:white; }"
//                                "QPushButton:hover{color:red;}");
    Return->setFont(QFont("Algerian",18));
    Return->setStyleSheet("QPushButton{background: transparent; color:white; }"
                                "QPushButton:hover{color:red;}");
    Score->setFont(QFont("Algerian",18));
    Score->setStyleSheet("QLabel{background: transparent; color:white; }"
                                );
    Win->setFont(QFont("Algerian",18));
    Win->setStyleSheet("QLabel{background: transparent; color:white; }"
                                );

    QVBoxLayout *lay = new QVBoxLayout;
    lay->addWidget(Win);
    lay->addWidget(Score);
    lay->addWidget(Return);
//    lay->addWidget(Next);

    setLayout(lay);

    setAutoFillBackground(true);
    QPalette pal;
    QPixmap pixmap(":/image/images/background.png");
    pal.setBrush(QPalette::Background, QBrush(pixmap));;
    setPalette(pal);
    serialconnect();
    for(int i = 0;i<2;i++)
    {
        temp.push_back(3.3);
    }
}

WinGame::WinGame(int x, int y){
    next = y;
    Return = new QPushButton("Return");
    Next = new QPushButton("Next");
    Score = new QLabel(this);
    Score->setText(QString(SCORE).arg(x));
    Win = new QLabel(this);
    Win->setText("YOU WIN");
    Next->setFont(QFont("Algerian",18));
    Next->setStyleSheet("QPushButton{background: transparent; color:white; }"
                                "QPushButton:hover{color:red;}");
    Return->setFont(QFont("Algerian",18));
    Return->setStyleSheet("QPushButton{background: transparent; color:white; }"
                                "QPushButton:hover{color:red;}");
    Score->setFont(QFont("Algerian",18));
    Score->setStyleSheet("QLabel{background: transparent; color:white; }"
                                );
    Win->setFont(QFont("Algerian",18));
    Win->setStyleSheet("QLabel{background: transparent; color:white; }"
                                );

    QVBoxLayout *lay = new QVBoxLayout;
    lay->addWidget(Win);
    lay->addWidget(Score);
    lay->addWidget(Return);
    lay->addWidget(Next);

    setLayout(lay);

    setAutoFillBackground(true);
    QPalette pal;
    QPixmap pixmap(":/image/images/background.png");
    pal.setBrush(QPalette::Background, QBrush(pixmap));;
    setPalette(pal);
    connect(Return,&QPushButton::clicked,this,&WinGame::ReturnClicked);
    connect(Next,&QPushButton::clicked,this,&WinGame::NextClicked);
    serialconnect();
    for(int i = 0;i<2;i++)
    {
        temp.push_back(3.3);
    }
}

WinGame::~WinGame()
{
}

void WinGame::NextClicked(){
    m_serialPort->clear();
    m_serialPort->close();
    if(next==1){
        Easy2 *e = new Easy2;
        e->show();
        this->close();
    }
    if(next==2){
        Hard2 *h = new Hard2;
        h->show();
        this->close();
    }
}
void WinGame::ReturnClicked(){
    m_serialPort->clear();
    m_serialPort->close();
    Choose *c = new Choose;
    c->show();
    this->close();
}

void WinGame::serialconnect()
{
    QList<QSerialPortInfo> serialPorts = QSerialPortInfo::availablePorts();
//    for(QSerialPortInfo serialPort : serialPorts){
//        ui->comComboBox->addItem(serialPort.portName());
//    }
    if(serialPorts.size() == 0)
    {
        return;
    }
    m_serialPort = new QSerialPort();//实例化串口类一个对象
    if(m_serialPort->isOpen())//如果串口已经打开了 先给他关闭了
    {
        m_serialPort->clear();
        m_serialPort->close();
    }

    //设置串口名字 假设我们上面已经成功获取到了 并且使用第一个
    m_serialPort->setPortName(serialPorts.at(0).portName());

    if(!m_serialPort->open(QIODevice::ReadWrite))//用ReadWrite 的模式尝试打开串口
    {
        //qDebug()<<ui->comComboBox->currentText()<<"打开失败!";
        return;
    }
    //打开成功
    m_serialPort->setBaudRate(QSerialPort::Baud9600,QSerialPort::AllDirections);//设置波特率和读写方向
    m_serialPort->setDataBits(QSerialPort::Data8);		//数据位为8位
    m_serialPort->setFlowControl(QSerialPort::NoFlowControl);//无流控制
    m_serialPort->setParity(QSerialPort::NoParity);	//无校验位
    m_serialPort->setStopBits(QSerialPort::OneStop); //一位停止位

    //连接信号槽 当下位机发送数据QSerialPortInfo 会发送个 readyRead 信号,我们定义个槽void receiveInfo()解析数据
    connect(m_serialPort,SIGNAL(readyRead()),this,SLOT(receiveInfo()));
}

void WinGame::receiveInfo()
{
    //数据格式:左摇杆x轴电压：左摇杆y轴电压：右摇杆x轴电压：右摇杆y轴电压：左按键电压：右按键电压
    //x.x:x.x:x.x:x.x:x.x:x.x
    QString info = m_serialPort->readAll();
    qDebug() << info;
    if(info.endsWith("\n")&&info.size()>=23)
    {
        QStringList infolist = info.split(":");
        int i = 0;
        QVector<float> data;
        for (auto it = infolist.begin();it<infolist.end();it++,i++)
        {
            data.push_back(infolist.at(i).toFloat());
        }
        qDebug() <<   "widget"<<data;

        if(abs(temp.at(0)-data.at(4))<0.2&&abs(temp.at(1)-data.at(5))<0.2)
        {
            temp.replace(0,3.3);
            temp.replace(1,3.3);
            return;
        }
        else
        {
            temp.replace(0,data.at(4));
            temp.replace(1,data.at(5));
        }
        if(QString::number(data.at(4)) != "3.3")
        {
            if(data.at(4)<0.2)
            {
                keybd_event(VK_UP,0,0,0);//模拟tab键被按下
                keybd_event(VK_UP,0,2,0);//模拟tab键被抬起
            }
            else if((data.at(4)-0.5)<0.2)
            {
                keybd_event(VK_LEFT,0,0,0);//模拟tab键被按下
                keybd_event(VK_LEFT,0,2,0);//模拟tab键被抬起
            }
            else if((data.at(4)-1.1)<0.2)
            {
                keybd_event(VK_RIGHT,0,0,0);//模拟tab键被按下
                keybd_event(VK_RIGHT,0,2,0);//模拟tab键被抬起
            }
            else if((data.at(4)-1.6)<0.2)
            {
                keybd_event(VK_DOWN,0,0,0);//模拟tab键被按下
                keybd_event(VK_DOWN,0,2,0);//模拟tab键被抬起
            }
            else if((data.at(4)-2.4)<0.2)
            {
                keybd_event(VK_ESCAPE,0,0,0);//模拟tab键被按下
                keybd_event(VK_ESCAPE,0,2,0);//模拟tab键被抬起
            }
        }
        else if(QString::number(data.at(5)) != "3.3")
        {
            if(data.at(5)<0.2)
            {
                keybd_event(VK_UP,0,0,0);//模拟tab键被按下
                keybd_event(VK_UP,0,2,0);//模拟tab键被抬起
            }
            else if((data.at(5)-0.5)<0.2)
            {
                keybd_event(VK_LEFT,0,0,0);//模拟tab键被按下
                keybd_event(VK_LEFT,0,2,0);//模拟tab键被抬起
            }
            else if((data.at(5)-1.1)<0.2)
            {
                keybd_event(VK_RIGHT,0,0,0);//模拟tab键被按下
                keybd_event(VK_RIGHT,0,2,0);//模拟tab键被抬起
            }
            else if((data.at(5)-1.6)<0.2)
            {
                keybd_event(VK_DOWN,0,0,0);//模拟tab键被按下
                keybd_event(VK_DOWN,0,2,0);//模拟tab键被抬起
            }
            else if((data.at(5)-2.4)<0.2)
            {
                keybd_event(VK_SPACE,0,0,0);//模拟tab键被按下
                keybd_event(VK_SPACE,0,2,0);//模拟tab键被抬起
            }
        }
    }
}
