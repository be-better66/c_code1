#include "choose.h"
#include <QPixmap>
#include <QGridLayout>
#include <ranking.h>

Choose::Choose(QWidget *parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_DeleteOnClose,true);
    resize(500,600);
    setWindowTitle("Choose");
    easy = new QPushButton("1");
    hard = new QPushButton("1");
    easy2 = new QPushButton("2");
    hard2 = new QPushButton("2");
    RankButton = new QPushButton("Ranking");
    Quit = new QPushButton("Quit");

    Quit->setFont(QFont("Algerian",18));
    Quit->setStyleSheet("QPushButton{background: transparent; color:white; }"
                                "QPushButton:hover{color:red;}");

    RankButton->setFont(QFont("Algerian",18));
    RankButton->setStyleSheet("QPushButton{background: transparent; color:white; }"
                                "QPushButton:hover{color:red;}");
    easy->setFont(QFont("Algerian",18));
    easy->setStyleSheet("QPushButton{background: transparent; color:white; }"
                                "QPushButton:hover{color:red;}");  
    easy2->setFont(QFont("Algerian",18));
    easy2->setStyleSheet("QPushButton{background: transparent; color:white; }"
                                "QPushButton:hover{color:red;}");
    hard->setFont(QFont("Algerian",18));
    hard->setStyleSheet("QPushButton{background: transparent; color:white; }"
                                "QPushButton:hover{color:red;}");
    hard2->setFont(QFont("Algerian",18));
    hard2->setStyleSheet("QPushButton{background: transparent; color:white; }"
                                "QPushButton:hover{color:red;}");
    label1 = new QLabel(this);
    label2 = new QLabel(this);

    label1->setFont(QFont("Algerian",18));
    label1->setStyleSheet("QLabel{background: transparent; color:white; }"
                                );
    label2->setFont(QFont("Algerian",18));
    label2->setStyleSheet("QLabel{background: transparent; color:white; }"
                                );

    label1->setText("Easy:");
    label2->setText("Hard:");

    Recored = new QPushButton("Recored");
    Recored->setGeometry(20,20,450,100);
    QHBoxLayout* lay1 = new QHBoxLayout;
    lay1->addWidget(easy);
    lay1->addWidget(easy2);

    QHBoxLayout* lay2 = new QHBoxLayout;
    lay2->addWidget(hard);
    lay2->addWidget(hard2);

    QGridLayout *mainlay = new QGridLayout;
    mainlay->addWidget(label1,0,0);
    mainlay->addWidget(RankButton,0,1);
    mainlay->addLayout(lay1,1,0);
    mainlay->addWidget(label2,2,0);
    mainlay->addLayout(lay2,3,0);
    mainlay->addWidget(Quit,3,1);

    setLayout(mainlay);

    setAutoFillBackground(true);
    QPalette pal;
    QPixmap pixmap(":/image/images/background.png");
    pal.setBrush(QPalette::Background, QBrush(pixmap));;
    setPalette(pal);


    connect(easy,&QPushButton::clicked,this,&Choose::easyClicked);
    connect(hard,&QPushButton::clicked,this,&Choose::hardClicked);
    connect(easy2,&QPushButton::clicked,this,&Choose::easy2Clicked);
    connect(hard2,&QPushButton::clicked,this,&Choose::hard2Clicked);
    connect(RankButton,&QPushButton::clicked,this,&Choose::RankClicked);
    connect(Quit,&QPushButton::clicked,this,&Choose::QuitClicked);
    serialconnect();
    for(int i = 0;i<2;i++)
    {
        temp.push_back(3.3);
    }
    delay_count = 3;
}

void Choose::easyClicked(){
    m_serialPort->clear();
    m_serialPort->close();
    qDebug() << "chooseclose";
    Easy *e = new Easy;
    e->show();
    this->close();
}


void Choose::easy2Clicked(){
    m_serialPort->clear();
    m_serialPort->close();
    Easy2 *e = new Easy2;
    e->show();
    this->close();
}

void Choose::hardClicked(){
    m_serialPort->clear();
    m_serialPort->close();
    Hard *h = new Hard;
    h->show();
    this->close();
}

void Choose::hard2Clicked(){
    m_serialPort->clear();
    m_serialPort->close();
    Hard2 *h = new Hard2;
    h->show();
    this->close();
}

void Choose::RankClicked(){
    m_serialPort->clear();
    m_serialPort->close();
    Ranking *r = new Ranking;
    r->show();
    this->close();
}

void Choose::QuitClicked(){
    this->close();
}

void Choose::serialconnect()
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

void Choose::receiveInfo()
{
    //数据格式:左摇杆x轴电压：左摇杆y轴电压：右摇杆x轴电压：右摇杆y轴电压：左按键电压：右按键电压
    //x.x:x.x:x.x:x.x:x.x:x.x
    QString info = m_serialPort->readAll();
    qDebug() << info;
    if(info.endsWith("\n")&&info.size()>=23)
    {
        if(delay_count!=0)
        {
            delay_count--;
            return;
        }
        QStringList infolist = info.split(":");
        int i = 0;
        QVector<float> data;
        for (auto it = infolist.begin();it<infolist.end();it++,i++)
        {
            data.push_back(infolist.at(i).toFloat());
        }
        qDebug() <<   "choose"<<data;

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


Choose::~Choose(){
}
