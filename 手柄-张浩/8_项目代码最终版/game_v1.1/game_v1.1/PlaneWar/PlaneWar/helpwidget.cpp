#include "helpwidget.h"
#include <QVBoxLayout>

HelpWidget::HelpWidget(QWidget *parent) :
    QWidget(parent)

{
    setAutoFillBackground(true);
    QPalette pal;
    QPixmap pixmap(":/image/images/background.png");
    pal.setBrush(QPalette::Background, QBrush(pixmap));;
    setPalette(pal);

    Label1 = new QLabel(this);
    Label1->setFont(QFont("Algerian",18));
    Label1->setStyleSheet("QLabel{background:transparent;color:white;}");

    Label2 = new QLabel(this);
    Label2->setFont(QFont("Algerian",18));
    Label2->setStyleSheet("QLabel{background:transparent;color:white;}");

    Label3 = new QLabel(this);
    Label3->setFont(QFont("Algerian",18));
    Label3->setStyleSheet("QLabel{background:transparent;color:white;}");

    Label4 = new QLabel(this);
    Label4->setFont(QFont("Algerian",18));
    Label4->setStyleSheet("QLabel{background:transparent;color:white;}");

    Label5 = new QLabel(this);
    Label5->setFont(QFont("Algerian",18));
    Label5->setStyleSheet("QLabel{background:transparent;color:white;}");

    Label6 = new QLabel(this);
    Label6->setFont(QFont("Algerian",18));
    Label6->setStyleSheet("QLabel{background:transparent;color:white;}");

    Label7 = new QLabel(this);
    Label7->setFont(QFont("Algerian",18));
    Label7->setStyleSheet("QLabel{background:transparent;color:white;}");

    Label8 = new QLabel(this);
    Label8->setFont(QFont("Algerian",18));
    Label8->setStyleSheet("QLabel{background:transparent;color:white;}");

    Label9 = new QLabel(this);
    Label9->setFont(QFont("Algerian",18));
    Label9->setStyleSheet("QLabel{background:transparent;color:white;}");

    Label10 = new QLabel(this);
    Label10->setFont(QFont("Algerian",18));
    Label10->setStyleSheet("QLabel{background:transparent;color:white;}");

    Label11 = new QLabel(this);
    Label11->setFont(QFont("Algerian",18));
    Label11->setStyleSheet("QLabel{background:transparent;color:white;}");

    Label1->setText("本游戏共分四个关卡，简单难度两关，困难难度两关");
    Label2->setText("在游戏中，你可以通过鼠标拖拽来操控飞机，通过键盘上的QWER来释放技能");
    Label3->setText("Q技能：发射一枚导弹，造成伤害为普通子弹的五倍，消耗三点技能值");
    Label4->setText("W技能：回复一点生命值，消耗三点技能值");
    Label5->setText("E技能：增加攻速为两倍，持续消耗技能值");
    Label6->setText("R技能：造成清屏伤害，消耗十点技能值");
    Label7->setText("     ");
    Label8->setText("游戏中的敌机分两类，一类是体型较小，在一条线上发射子弹的普通飞机，生命值为1");
    Label9->setText( "另一类是提醒较大，在三条线上发射子弹的Boss型飞机，生命值为10");
    Label10->setText("在游戏中，你可以通过Esc键退出游戏回到选择关卡界面");
    Label11->setText("选关界面有排行榜按钮，可以查看过往记录");

    QVBoxLayout *lay = new QVBoxLayout;
    lay->addWidget(Label1);
    lay->addWidget(Label2);
    lay->addWidget(Label3);
    lay->addWidget(Label4);
    lay->addWidget(Label5);
    lay->addWidget(Label6);
    lay->addWidget(Label7);
    lay->addWidget(Label8);
    lay->addWidget(Label9);
    lay->addWidget(Label10);
    lay->addWidget(Label11);

    setLayout(lay);
}

HelpWidget::~HelpWidget()
{
}
