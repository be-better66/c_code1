#include "easy2.h"
#include <ctime>
#include <cmath>
#include <QMessageBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QString>
#include <iostream>
#include <ranking.h>
using namespace std;
Easy2::Easy2(QWidget *parent) :
    QWidget(parent)

{
    setAttribute(Qt::WA_DeleteOnClose,true);
    resize(500,600);
    Score = 0;
    setAutoFillBackground(true);
    QPalette pal;
    QPixmap pixmap(":/image/images/background.png");
    pal.setBrush(QPalette::Background, QBrush(pixmap));
    setPalette(pal);
    life = new QLabel(this);
    skill = new QLabel(this);
    score = new QLabel(this);
    life->setFont(QFont("Algerian",16));
    life->setStyleSheet("QLabel{background:transparent;color:white;}");
    skill->setFont(QFont("Algerian",16));
    skill->setStyleSheet("QLabel{background:transparent;color:white;}");
    score->setFont(QFont("Algerian",16));
    score->setStyleSheet("QLabel{background:transparent;color:white;}");
    QVBoxLayout *lay = new QVBoxLayout;
    lay->addWidget(life);
    lay->addWidget(skill);
    lay->addWidget(score);
    setLayout(lay);
    win = false;
    winPlayed = false;
    Epressed = false;
    initial();
    serialconnect();
}

Easy2::~Easy2()
{
    setAttribute(Qt::WA_DeleteOnClose,true);
}

void Easy2::serialconnect()
{
    QList<QSerialPortInfo> serialPorts = QSerialPortInfo::availablePorts();
//    for(QSerialPortInfo serialPort : serialPorts){
//        ui->comComboBox->addItem(serialPort.portName());
//    }
    qDebug() <<"easy";
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
        qDebug()<<"打开失败!";
        return;
    }
    //打开成功
    m_serialPort->setBaudRate(QSerialPort::Baud9600,QSerialPort::AllDirections);//设置波特率和读写方向
    m_serialPort->setDataBits(QSerialPort::Data8);		//数据位为8位
    m_serialPort->setFlowControl(QSerialPort::NoFlowControl);//无流控制
    m_serialPort->setParity(QSerialPort::NoParity);	//无校验位
    m_serialPort->setStopBits(QSerialPort::OneStop); //一位停止位
    qDebug() <<"easy";
    //连接信号槽 当下位机发送数据QSerialPortInfo 会发送个 readyRead 信号,我们定义个槽void receiveInfo()解析数据
    connect(m_serialPort,SIGNAL(readyRead()),this,SLOT(receiveInfo()));
}

void Easy2::receiveInfo()
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
        qDebug() <<   MyPlane.x<<MyPlane.y;

        if(QString::number(data.at(0)) != "1.7")
        {
            if((data.at(0)-1.7)>0.1)
            {
                if(MyPlane.x>0&&MyPlane.x<420)
                {
                    MyPlane.x += 10;
                }
                if(MyPlane.x>=420)
                {
                    MyPlane.x = 415;
                }
            }
            else if((1.7-data.at(0))>0.1)
            {
                if(MyPlane.x>0&&MyPlane.x<420)
                {
                    MyPlane.x -= 10;
                }
                if(MyPlane.x<=0)
                {
                    MyPlane.x = 10;
                }
            }
            update();
        }
        if(QString::number(data.at(1)) != "1.7")
        {
            if((data.at(1)-1.7)>0.1)
            {
                if(MyPlane.y>0&&MyPlane.y<520)
                {
                    MyPlane.y -= 10;
                }
                if(MyPlane.y<=0)
                {
                    MyPlane.y = 10;
                }
            }
            else if((1.7-data.at(1))>0.1)
            {
                if(MyPlane.y>0&&MyPlane.y<520)
                {
                    MyPlane.y += 10;
                }
                if(MyPlane.y>=520)
                {
                    MyPlane.y = 510;
                }
            }
            update();
        }
        if(QString::number(data.at(2)) != "1.7")
        {
            if((data.at(2)-1.7)>0.1)
            {
                if(MyPlane.x>0&&MyPlane.x<420)
                {
                    MyPlane.x += 10;
                }
                if(MyPlane.x>=420)
                {
                    MyPlane.x = 415;
                }
            }
            else if((1.7-data.at(2))>0.1)
            {
                if(MyPlane.x>0&&MyPlane.x<420)
                {
                    MyPlane.x -= 10;
                }
                if(MyPlane.x<=0)
                {
                    MyPlane.x = 10;
                }
            }
            update();
        }
        if(QString::number(data.at(3)) != "1.7")
        {
            if((data.at(3)-1.7)>0.1)
            {
                if(MyPlane.y>0&&MyPlane.y<520)
                {
                    MyPlane.y -= 10;
                }
                if(MyPlane.y<=0)
                {
                    MyPlane.y = 10;
                }
            }
            else if((1.7-data.at(3))>0.1)
            {
                if(MyPlane.y>0&&MyPlane.y<520)
                {
                    MyPlane.y += 10;
                }
                if(MyPlane.y>=520)
                {
                    MyPlane.y = 510;
                }
            }
            update();
        }
        if(QString::number(data.at(4)) != "3.3")
        {
            if(data.at(4)<0.2)//Q键
            {
                keybd_event(0x51,0,0,0);//模拟tab键被按下
                keybd_event(0x51,0,KEYEVENTF_KEYUP,0);//模拟tab键被抬起
            }
            else if((data.at(4)-0.5)<0.2)//W键
            {
                keybd_event(0x57,0,0,0);//模拟tab键被按下
                keybd_event(0x57,0,KEYEVENTF_KEYUP,0);//模拟tab键被抬起
            }
            else if((data.at(4)-1.1)<0.2)//E键
            {
                keybd_event(0x45,0,0,0);//模拟tab键被按下
                keybd_event(0x45,0,KEYEVENTF_KEYUP,0);//模拟tab键被抬起
            }
            else if((data.at(4)-1.6)<0.2)//R键
            {
                keybd_event(0x52,0,0,0);//模拟tab键被按下
                keybd_event(0x52,0,KEYEVENTF_KEYUP,0);//模拟tab键被抬起
            }
            else if((data.at(4)-2.4)<0.2)
            {
                keybd_event(VK_ESCAPE,0,0,0);//模拟tab键被按下
                keybd_event(VK_ESCAPE,0,2,0);//模拟tab键被抬起
            }
            update();
        }
        if(QString::number(data.at(5)) != "3.3")
        {
            if(data.at(5)<0.2)//Q键
            {
                keybd_event(0x51,0,0,0);//模拟tab键被按下
                keybd_event(0x51,0,KEYEVENTF_KEYUP,0);//模拟tab键被抬起
            }
            else if((data.at(5)-0.5)<0.2)//W键
            {
                keybd_event(0x57,0,0,0);//模拟tab键被按下
                keybd_event(0x57,0,KEYEVENTF_KEYUP,0);//模拟tab键被抬起
            }
            else if((data.at(5)-1.1)<0.2)//E键
            {
                keybd_event(0x45,0,0,0);//模拟tab键被按下
                keybd_event(0x45,0,KEYEVENTF_KEYUP,0);//模拟tab键被抬起
            }
            else if((data.at(5)-1.6)<0.2)//R键
            {
                keybd_event(0x52,0,0,0);//模拟tab键被按下
                keybd_event(0x52,0,KEYEVENTF_KEYUP,0);//模拟tab键被抬起
            }
            else if((data.at(5)-2.4)<0.2)
            {
                keybd_event(VK_SPACE,0,0,0);//模拟tab键被按下
                keybd_event(VK_SPACE,0,2,0);//模拟tab键被抬起
            }
            update();
        }
    }
}

void Easy2::initial(){
     Timer.setInterval(10);
     startGame();
     EnemyRecored1 = 0;
     EnemyRecored2 = 0;
     supplyRecored = 0;
     srand((unsigned int)time(NULL));
}

void Easy2::paintEvent(QPaintEvent *){
    QPainter painter(this);
    painter.drawPixmap(MyPlane.x,MyPlane.y,MyPlane.myplane);
    for(int i=0;i<30;i++)
        if(!MyPlane.myBullet[i].isFree)
            painter.drawPixmap(MyPlane.myBullet[i].x,MyPlane.myBullet[i].y,MyPlane.myBullet[i].bullet);
    for(int i=0;i<5;i++)
        if(!MyPlane.BigBullet[i].isFree)
            painter.drawPixmap(MyPlane.BigBullet[i].x,MyPlane.BigBullet[i].y,MyPlane.BigBullet[i].bigbullet);
    for(int i=0;i<20;i++)
        if(!lifesupply[i].isFree)
            painter.drawPixmap(lifesupply[i].x,lifesupply[i].y,lifesupply[i].lifesupply);
//敌机及其子弹动画
    for(int i=0;i<30;i++){
        if(!Enemy1[i].isFree){
            if(!Enemy1[i].isDestroyed)
                painter.drawPixmap(Enemy1[i].x,Enemy1[i].y,Enemy1[i].enemy1);
            else
                if(!Enemy1[i].bomb.isPlayde)
                    painter.drawPixmap(Enemy1[i].x,Enemy1[i].y,Enemy1[i].bomb.bombPix[Enemy1[i].bomb.index]);
        }
        for(int j=0;j<30;j++){
            if(!Enemy1[i].enemy1Bullet[j].isFree)
                painter.drawPixmap(Enemy1[i].enemy1Bullet[j].x,Enemy1[i].enemy1Bullet[j].y,
                                   Enemy1[i].enemy1Bullet[j].EnemyBullet);
        }
    }
//Boss及其子弹动画
    for(int i=0;i<5;i++)
        if(!Enemy2[i].isFree){
            if(!Enemy2[i].isDestroyed)
                painter.drawPixmap(Enemy2[i].x,Enemy2[i].y,Enemy2[i].enemy2);
            else
                if(!Enemy2[i].bomb.isPlayde)
                    painter.drawPixmap(Enemy2[i].x+50,Enemy2[i].y,Enemy2[i].bomb.bombPix[Enemy2[i].bomb.index]);
            for(int j=0;j<30;j++){
                if(!Enemy2[i].enemy2Bullet1[j].isFree)
                    painter.drawPixmap(Enemy2[i].enemy2Bullet1[j].x,Enemy2[i].enemy2Bullet1[j].y,
                                       Enemy2[i].enemy2Bullet1[j].EnemyBullet2);
            }
            for(int j=0;j<30;j++){
                if(!Enemy2[i].enemy2Bullet2[j].isFree)
                    painter.drawPixmap(Enemy2[i].enemy2Bullet2[j].x,Enemy2[i].enemy2Bullet2[j].y,
                                       Enemy2[i].enemy2Bullet2[j].EnemyBullet2);
            }
            for(int j=0;j<30;j++){
                if(!Enemy2[i].enemy2Bullet3[j].isFree)
                    painter.drawPixmap(Enemy2[i].enemy2Bullet3[j].x,Enemy2[i].enemy2Bullet3[j].y,
                                       Enemy2[i].enemy2Bullet3[j].EnemyBullet2);
            }
        }
}


void Easy2::mouseMoveEvent(QMouseEvent *E){
    int x = E->x()-35;
    int y = E->y()-40;
    if(x>0&&x<415)
        MyPlane.x = x;
    if(y>0&&y<505)
        MyPlane.y = y;
    update();
}
                                                                //键盘事件，释放技能
void Easy2::keyPressEvent(QKeyEvent *E){
    if(E->key()==Qt::Key_Q){                                    //Q技能
        if(MyPlane.skill>=3)
            for(int i = 0;i<5;i++)
                if(MyPlane.BigBullet[i].isFree){
                    MyPlane.skill-=3;
                    MyPlane.BigBullet[i].x = MyPlane.x+40;
                    MyPlane.BigBullet[i].y = MyPlane.y-10;
                    MyPlane.BigBullet[i].isFree = false;
                    break;
                }
    }
    if(E->key()==Qt::Key_W){                                    //W技能
        if(MyPlane.life==10)
            return;
        else
            if(MyPlane.skill>=3){
                MyPlane.skill-=3;
                MyPlane.life++;
        }
    }
    if(E->key()==Qt::Key_E){                                    //E技能
            if(Epressed){
                Epressed = false;
                MyPlane.interval = 30;
            }
            else{
                Epressed = true;
                if(MyPlane.skill>0)
                    MyPlane.interval = 15;
            }
    }
    if(E->key()==Qt::Key_R)                                      //R技能
        if(MyPlane.skill>=10){
            for(int i=0;i<30;i++){
                if(!Enemy1[i].isFree&&!Enemy1[i].isDestroyed){
                    MyPlane.skill+=1;
                    Score+=10;
                    Enemy1[i].isDestroyed = true;
                }
            }
            for(int i=0;i<5;i++){
                if(!Enemy2[i].isDestroyed&&!Enemy2[i].isFree){
                    MyPlane.skill+=1;
                    Score+=20;
                    Enemy2[i].isDestroyed = true;
                }
            }
            MyPlane.skill-=10;
        }
    if(E->key()==Qt::Key_Escape){
        m_serialPort->clear();
        m_serialPort->close();
        MyPlane.isPlayed = true;
        winPlayed = true;
        Choose *c = new Choose;
        c->show();
        this->close();
    }
    update();
}
                                                        //游戏开始
void Easy2::startGame(){

    Timer.start();
    connect(&Timer , &QTimer::timeout,[=](){
        EnemyShow();

        BossShow();

        SupplyShow();

        updatePositino();

        collisionDetetion();

        endGame();

        GameWin();

        update();
    });
}
                                                            //游戏失败
void Easy2::endGame(){
    if(MyPlane.life==0&&!MyPlane.isPlayed&&!winPlayed){
        m_serialPort->clear();
        m_serialPort->close();
        MyPlane.isPlayed = true;
        EndGame *e = new EndGame(Score);
        cout<<MyPlane.life;
        cout<<MyPlane.isPlayed;
        cout<<winPlayed;
        e->show();
        this->close();
    }
}
                                                            //游戏胜利
void Easy2::GameWin(){
    if(win){
        if(MyPlane.life>0)
            if(!MyPlane.isPlayed)
                if(!winPlayed){
                    m_serialPort->clear();
                    m_serialPort->close();
                    winPlayed = true;
                    WinGame *w = new WinGame(Score,2);
                    cout<<"Winnnnnnnnn";
                    cout<<MyPlane.life;
                    cout<<MyPlane.isPlayed;
                    cout<<winPlayed;
                    cout<<win;
                    Ranking *r = new Ranking;
                    r->updateRanking("Easy2",Score);
                    w->show();
                    this->close();
        }
    }
}

                                                            //更新游戏相关信息
void Easy2::updatePositino(){
    MyPlane.shoot();
    for(int i=0;i<30;i++)
        MyPlane.myBullet[i].updatePosition();
    for(int i=0;i<5;i++)
        MyPlane.BigBullet[i].updatePosition();
                                                            //敌机射击与运动
    for(int i=0;i<30;i++){
        if(!Enemy1[i].isFree&&!Enemy1[i].isDestroyed){
            Enemy1[i].shoot();
            Enemy1[i].updatePosition();
        }
        if(Enemy1[i].isDestroyed&&!Enemy1[i].isFree)
            Enemy1[i].bomb.updateInfo();
        for(int j=0;j<30;j++)
            Enemy1[i].enemy1Bullet[j].EnemyUpdatePosition();
     }
    for(int i=0;i<20;i++)
       lifesupply[i].EnemyUpdatePosition();
    life->setText(QString(LIFE).arg(MyPlane.life));
    score->setText(QString(SCORE).arg(Score));
    skill->setText(QString(SKILL).arg(MyPlane.skill));
                                                            //Boss射击与运动
    for(int i=0;i<5;i++){
        if(!Enemy2[i].isFree&&!Enemy2[i].isDestroyed){
            Enemy2[i].shoot();
            Enemy2[i].updatePosition();
        }
        if(Enemy2[i].isDestroyed&&!Enemy2[i].isFree)
            Enemy2[i].bomb.updateInfo();
        for(int j=0;j<30;j++){
            Enemy2[i].enemy2Bullet1[j].EnemyUpdatePositionLeft();
            Enemy2[i].enemy2Bullet2[j].EnemyUpdatePosition();
            Enemy2[i].enemy2Bullet3[j].EnemyUpdatePositionRight();
        }
    }
}


                                                    //物资出场
void Easy2::SupplyShow(){
    supplyRecored++;
    if(supplyRecored<200)
        return;
    supplyRecored = 0;
    for(int i=0;i<20;i++)
        if(lifesupply[i].isFree){
            lifesupply[i].isFree = false;
            lifesupply[i].x = rand()%(500-lifesupply[i].lifesupply.width());
            lifesupply[i].y = 0;
            break;
        }
}

                                                    //Boss出场
void Easy2::BossShow(){
    EnemyRecored2++;
    if(EnemyRecored2<500)
        return;
    EnemyRecored2=0;
    for(int i=0;i<5;i++)
        if(Enemy2[i].isFree){
            Enemy2[i].isFree = false;
            Enemy2[i].x = rand()%(500-Enemy2[i].enemy2.width());
            Enemy2[i].y = 0;
            break;
        }
}

                                                        //敌机出场
void Easy2::EnemyShow(){
    EnemyRecored1++;
    if(EnemyRecored1<150)
        return;
    EnemyRecored1=0;
    for(int i=0;i<30;i++)
        if(Enemy1[i].isFree&&!Enemy1[i].isDestroyed){
            Enemy1[i].isFree = false;
            Enemy1[i].x = rand()%(500-Enemy1[i].enemy1.width());
            Enemy1[i].y = 0;
            break;
        }
}

//碰撞检测
void Easy2::collisionDetetion(){
                                                        //遍历敌机
    for(int i=0;i<30;i++){
        if(!Enemy1[i].isFree){
            if(getDistanceEAM(Enemy1[i],MyPlane)<30)
            {
                m_serialPort->write("YM1:");
                MyPlane.life--;
                Enemy1[i].isDestroyed = true;
                Enemy1[i].bomb.x = Enemy1[i].x;
                Enemy1[i].bomb.y = Enemy1[i].y;
            }
            for(int j=0;j<30;j++){                      //遍历我机子弹
                if(MyPlane.myBullet[j].isFree)
                    continue;
                if(getDistanceBAE(MyPlane.myBullet[j],Enemy1[i])<30&&!Enemy1[i].isDestroyed){
                    MyPlane.skill+=1;
                    Score+=10;
                    MyPlane.myBullet[j].isFree = true;
                    Enemy1[i].isDestroyed = true;
                }
            }
            for(int j=0;j<5;j++){                      //遍历我机子弹
                if(MyPlane.BigBullet[j].isFree)
                    continue;
                if(getDistanceBAE(MyPlane.BigBullet[j],Enemy1[i])<30&&!Enemy1[i].isDestroyed){
                    MyPlane.skill+=1;
                    Score+=10;
                    MyPlane.BigBullet[j].isFree = true;
                    Enemy1[i].isDestroyed = true;
                }
            }
        }
        for(int j=0;j<30;j++){                          //遍历敌机子弹
            if(Enemy1[i].enemy1Bullet[j].isFree)
                continue;
            if(getDistanceBAM(Enemy1[i].enemy1Bullet[j],MyPlane)<30){
                m_serialPort->write("YM1:");
                MyPlane.life--;
                Enemy1[i].enemy1Bullet[j].isFree = true;
            }
        }

    }
                                                            //遍历物资
    for(int i=0;i<20;i++){
        if(!lifesupply[i].isFree)
            if(getDistanceBAM(lifesupply[i],MyPlane)<30){
                if(MyPlane.life==10){
                    lifesupply[i].isFree = true;
                    continue;
                }
                else{
                    lifesupply[i].isFree = true;
                    MyPlane.life++;
                }

            }
    }
                                                            //遍历Boss
    for(int i=0;i<5;i++){
        if(!Enemy2[i].isFree&&!Enemy2[i].isDestroyed){
            for(int j=0;j<30;j++){                          //遍历我机子弹
                if(MyPlane.myBullet[j].isFree)
                    continue;
                if(MyPlane.myBullet[j].x<Enemy2[i].x+Enemy2[i].enemy2.width()&&MyPlane.myBullet[j].x>Enemy2[i].x&&
                   MyPlane.myBullet[j].y<Enemy2[i].y){
                    Enemy2[i].life--;
                    MyPlane.myBullet[j].isFree = true;
                    if(Enemy2[i].life<=0){
                        MyPlane.skill+=1;
                        Score+=20;
                        Enemy2[i].isDestroyed = true;
                    }
                }
            }
        }
            for(int j=0;j<5;j++){
                if(MyPlane.BigBullet[j].isFree)
                    continue;
                if(MyPlane.BigBullet[j].x<Enemy2[i].x+Enemy2[i].enemy2.width()&&MyPlane.BigBullet[j].x>Enemy2[i].x&&
                   MyPlane.BigBullet[j].y<Enemy2[i].y){
                    Enemy2[i].life-=5;
                    MyPlane.BigBullet[j].isFree = true;
                    if(Enemy2[i].life<=0){
                        MyPlane.skill+=1;
                        Score+=20;
                        Enemy2[i].isDestroyed = true;
                    }
            }
        }
        for(int j=0;j<30;j++){
            if(Enemy2[i].enemy2Bullet1[j].isFree)
                continue;
            if(getDistanceBAM(Enemy2[i].enemy2Bullet1[j],MyPlane)<30){
                m_serialPort->write("YM1:");
                MyPlane.life--;
                Enemy2[i].enemy2Bullet1[j].isFree = true;
            }
            if(Enemy2[i].enemy2Bullet2[j].isFree)
                continue;
            if(getDistanceBAM(Enemy2[i].enemy2Bullet2[j],MyPlane)<30){
                m_serialPort->write("YM1:");
                MyPlane.life--;
                Enemy2[i].enemy2Bullet2[j].isFree = true;
            }
            if(Enemy2[i].enemy2Bullet3[j].isFree)
                continue;
            if(getDistanceBAM(Enemy2[i].enemy2Bullet3[j],MyPlane)<30){
                m_serialPort->write("YM1:");
                MyPlane.life--;
                Enemy2[i].enemy2Bullet3[j].isFree = true;
            }
        }
    }
    for(int i=0;i<30;i++)
        if(!Enemy1[i].isDestroyed)
            return;
    for(int i=0;i<5;i++)
        if(!Enemy2[i].isDestroyed)
            return;
    win = true;

}

//计算我机子弹和敌机的距离
int Easy2::getDistanceBAE(Bullet b,EnemyPlane1 E){
    return sqrt((b.x-20-E.x)*(b.x-20-E.x)+(b.y+20-E.y)*(b.y+20-E.y));
}

//计算敌机子弹与我机的距离
int Easy2::getDistanceBAM(Bullet b,Plane M){
    return sqrt((b.x-40-M.x)*(b.x-40-M.x)+(b.y-40-M.y)*(b.y-40-M.y));
}

//计算敌机与我机的距离
int Easy2::getDistanceEAM(EnemyPlane1 E, Plane M){
    return sqrt((E.x-M.x)*(E.x-M.x)+(E.y-M.y)*(E.y-M.y));
}
