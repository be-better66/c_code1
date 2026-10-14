#include "ranking.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPixmap>
#include <QPalette>
#include <choose.h>
Ranking::Ranking(QWidget *parent) :
    QWidget(parent)
{
    setAutoFillBackground(true);
    QPalette pal;
    QPixmap pixmap(":/image/images/background.png");
    pal.setBrush(QPalette::Background, QBrush(pixmap));;
    setPalette(pal);

    QString string[5];

    Return = new QPushButton("Return");
    Return->setFont(QFont("Algerian",16));
    Return->setStyleSheet("QLabel{background:transparent;color:white;}");

    Label1 = new QLabel(this);
    Label1->setText("Ranking    Easy1   Easy2   Hard1   Hard2");
    Label1->setFont(QFont("Algerian",16));
    Label1->setStyleSheet("QLabel{background:transparent;color:white;}");

    Label2 = new QLabel(this);
    Label2->setFont(QFont("Algerian",16));
    Label2->setStyleSheet("QLabel{background:transparent;color:white;}");

    Label3 = new QLabel(this);
    Label3->setFont(QFont("Algerian",16));
    Label3->setStyleSheet("QLabel{background:transparent;color:white;}");

    Label4 = new QLabel(this);
    Label4->setFont(QFont("Algerian",16));
    Label4->setStyleSheet("QLabel{background:transparent;color:white;}");

    Label5 = new QLabel(this);
    Label5->setFont(QFont("Algerian",16));
    Label5->setStyleSheet("QLabel{background:transparent;color:white;}");

    Label6 = new QLabel(this);
    Label6->setFont(QFont("Algerian",16));
    Label6->setStyleSheet("QLabel{background:transparent;color:white;}");

    if(QSqlDatabase::contains("qt_sql_default_connection"))
        database = QSqlDatabase::database("qt_sql_default_connection");
    else{
        database = QSqlDatabase::addDatabase("QSQLITE");
        database.setDatabaseName("Rank.db");
    }
    if (!database.open())
        qDebug() << "Error: Failed to connect database." << database.lastError();
    else{
        QSqlQuery sql_query(database);
        QString create_sql = "create table Score (Ranking int primary key, Easy1 int, Easy2 int, Hard1 int, Hard2 int)";
        sql_query.prepare(create_sql);
        if(!sql_query.exec())
            qDebug() << "Error: Fail to create table." << sql_query.lastError();
        else
            qDebug() << "Table created!";
        sql_query.exec("INSERT INTO Score VALUES(1, 0, 0, 0, 0)");
        sql_query.exec("INSERT INTO Score VALUES(2, 0, 0, 0, 0)");
        sql_query.exec("INSERT INTO Score VALUES(3, 0, 0, 0, 0)");
        sql_query.exec("INSERT INTO Score VALUES(4, 0, 0, 0, 0)");
        sql_query.exec("INSERT INTO Score VALUES(5, 0, 0, 0, 0)");
        QString select_all_sql = "select * from Score";
        sql_query.prepare(select_all_sql);
        if(!sql_query.exec())
        {
            qDebug()<<sql_query.lastError();
        }
        else
        {
            for(int i=0;sql_query.next();i++)
            {
                int Rank = sql_query.value(0).toInt();
                int Easy1 = sql_query.value(1).toInt();
                int Easy2 = sql_query.value(2).toInt();
                int Hard1 = sql_query.value(3).toInt();
                int Hard2 = sql_query.value(4).toInt();
                string[i]=QString("%1           %2      %3      %4      %5").arg(Rank).arg(Easy1).arg(Easy2).arg(Hard1).arg(Hard2);
            }
        }
        Label2->setText(string[0]);
        Label3->setText(string[1]);
        Label4->setText(string[2]);
        Label5->setText(string[3]);
        Label6->setText(string[4]);
        QVBoxLayout *lay = new QVBoxLayout;
        lay->addWidget(Label1);
        lay->addWidget(Label2);
        lay->addWidget(Label3);
        lay->addWidget(Label4);
        lay->addWidget(Label5);
        lay->addWidget(Label6);
        lay->addWidget(Return);

        setLayout(lay);
    }

    connect(Return,&QPushButton::clicked,this,&Ranking::ReturnClicked);
    serialconnect();
    for(int i = 0;i<2;i++)
    {
        temp.push_back(3.3);
    }
}


void Ranking::ReturnClicked(){
    m_serialPort->clear();
    m_serialPort->close();
    Choose *c = new Choose;
    c->show();
    this->close();
}

void Ranking::serialconnect()
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

void Ranking::receiveInfo()
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

Ranking::~Ranking()
{
}

void Ranking::updateRanking(QString s, int Score){
    if(QSqlDatabase::contains("qt_sql_default_connection"))
        database = QSqlDatabase::database("qt_sql_default_connection");
    else{
        database = QSqlDatabase::addDatabase("QSQLITE");
        database.setDatabaseName("Rank.db");
    }
    if (!database.open())
        qDebug() << "Error: Failed to connect database." << database.lastError();
    else{
        QSqlQuery sql_query(database);
        if(s=="Easy1"){
            int rank;
            int scores[5];
            for(int i=0;i<5;i++)
                scores[i]=0;
            QString select_sql = "select Ranking, Easy1 from Score";
            if(!sql_query.exec(select_sql))
            {
                qDebug()<<sql_query.lastError();
            }
            else{
                while(sql_query.next()){
                    rank = sql_query.value(0).toInt();
                    scores[rank-1] = sql_query.value(1).toInt();
                }
            }
            for(int i=0;i<5;i++){
                if(scores[i]>=Score)
                    continue;
                else{
                    for(int j=4;j>i;j--)
                        scores[j] = scores[j-1];
                    scores[i] = Score;
                    break;
                }
            }
            for(int i=0;i<5;i++){
                QString update_sql = "update Score set Easy1 = :Easy1 where Ranking = :Ranking";
                sql_query.prepare(update_sql);
                sql_query.bindValue(":Easy1", scores[i]);
                sql_query.bindValue(":Ranking", i+1);
                if(!sql_query.exec())
                {
                    qDebug() << sql_query.lastError();
                }
                else
                {
                    qDebug() << "updated!";
                }
            }
        }
        if(s=="Easy2"){
            int rank;
            int scores[5];
            for(int i=0;i<5;i++)
                scores[i]=0;
            QString select_sql = "select Ranking, Easy2 from Score";
            sql_query.prepare(select_sql);
            while(sql_query.next()){
                rank = sql_query.value(0).toInt();
                scores[rank-1] = sql_query.value(1).toInt();
                if(!sql_query.exec())
                {
                    qDebug() << sql_query.lastError();
                }
                else
                {
                    qDebug() << "updated!";
                }
            }
            for(int i=0;i<5;i++){
                if(scores[i]>=Score)
                    continue;
                else{
                    for(int j=4;j>i;j--)
                        scores[j] = scores[j-1];
                    scores[i] = Score;
                    break;
                }
            }
            for(int i=0;i<5;i++){
                QString update_sql = "update Score set Easy2 = :Easy2 where Ranking = :Ranking";
                sql_query.prepare(update_sql);
                sql_query.bindValue(":Easy2", scores[i]);
                sql_query.bindValue(":Ranking", i+1);
                if(!sql_query.exec())
                {
                    qDebug() << sql_query.lastError();
                }
                else
                {
                    qDebug() << "updated!";
                }
            }
        }
        if(s=="Hard1"){
            int rank;
            int scores[5];
            for(int i=0;i<5;i++)
                scores[i]=0;
            QString select_sql = "select Ranking, Hard1 from Score";
            if(!sql_query.exec(select_sql))
            {
                qDebug()<<sql_query.lastError();
            }
            else{
                for(int i=0;sql_query.next();i++){
                    rank = sql_query.value(0).toInt();
                    scores[i] = sql_query.value(1).toInt();
                    qDebug()<<scores[i];
                }
            }
            for(int i=0;i<5;i++){
                if(scores[i]>=Score)
                    continue;
                else{
                    for(int j=4;j>i;j--)
                        scores[j] = scores[j-1];
                    scores[i] = Score;
                    break;
                }
                qDebug()<<scores[i];
            }
            for(int i=0;i<5;i++){
                QString update_sql = "update Score set Hard1 = :Hard1 where Ranking = :Ranking";
                sql_query.prepare(update_sql);
                sql_query.bindValue(":Hard1", scores[i]);
                sql_query.bindValue(":Ranking", i+1);
                if(!sql_query.exec())
                {
                    qDebug() << sql_query.lastError();
                }
                else
                {
                    qDebug() << "updated!";
                }
            }
        }
        if(s=="Hard2"){
            int rank;
            int scores[5];
            for(int i=0;i<5;i++)
                scores[i]=0;
            QString select_sql = "select Ranking, Hard2 from Score";
            sql_query.prepare(select_sql);
            while(sql_query.next()){
                rank = sql_query.value(0).toInt();
                scores[rank-1] = sql_query.value(1).toInt();
                if(!sql_query.exec())
                {
                    qDebug() << sql_query.lastError();
                }
                else
                {
                    qDebug() << "updated!";
                }
            }
            for(int i=0;i<5;i++){
                if(scores[i]>=Score)
                    continue;
                else{
                    for(int j=4;j>i;j--)
                        scores[j] = scores[j-1];
                    scores[i] = Score;
                    break;
                }
            }
            for(int i=0;i<5;i++){
                QString update_sql = "update Score set Hard2 = :Hard2 where Ranking = :Ranking";
                sql_query.prepare(update_sql);
                sql_query.bindValue(":Hard2", scores[i]);
                sql_query.bindValue(":Ranking", i+1);
                if(!sql_query.exec())
                {
                    qDebug() << sql_query.lastError();
                }
                else
                {
                    qDebug() << "updated!";
                }
            }
        }
    }
}
