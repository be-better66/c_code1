#ifndef HARD_H
#define HARD_H

#include <QWidget>
#include <plane.h>
#include <QPainter>
#include <QPaintEvent>
#include <QKeyEvent>
#include <bullet.h>
#include <QTimer>
#include <QMouseEvent>
#include <enemyplane1.h>
#include <enemyplane2.h>
#include <QLabel>
#define SCORE "Score: %1"
#define LIFE  "Life:  %1"
#define SKILL "Skill: %1"
#include <endgame.h>
#include <wingame.h>
#include <choose.h>
#include <QSerialPortInfo>
#include <QSerialPort>
#include "Windows.h"//这是引入Windows操作系统的API

class Hard : public QWidget
{
    Q_OBJECT
public:
    bool win;
    bool winPlayed;
    int Score;
    bool Epressed;
    QLabel* score;
    Bullet lifesupply[20];
    QLabel* life;
    QLabel* skill;
    Plane MyPlane;
    EnemyPlane1 Enemy1[20];
    EnemyPlane2 Enemy2[5];
    int supplyRecored;
    int EnemyRecored1;
    int EnemyRecored2;
    QTimer Timer;
    void initial();
    void startGame();
    void endGame();
    void updatePositino();
    void paintEvent(QPaintEvent *E);
    void mouseMoveEvent(QMouseEvent *E);
    void BossShow();
    void EnemyShow();
    void SupplyShow();
    void GameWin();
    void collisionDetetion();
    int getDistanceBAE(Bullet b,EnemyPlane1 E);
    int getDistanceBAB(Bullet b,EnemyPlane2 B);
    int getDistanceBAM(Bullet b,Plane M);
    int getDistanceEAM(EnemyPlane1 E,Plane M);
    void keyPressEvent(QKeyEvent *E);
    explicit Hard(QWidget *parent = 0);
    ~Hard();
    QSerialPort* m_serialPort;

private slots:
    void serialconnect();
    void receiveInfo();
};

#endif // HARD_H
