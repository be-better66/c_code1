#ifndef ENEMYPLANE2_H
#define ENEMYPLANE2_H
#include <QPixmap>
#include <bullet.h>
#include <bomb.h>
class EnemyPlane2
{
public:
    EnemyPlane2();
    Bomb bomb;
    int x;
    int y;
    int speed;
    bool isDestroyed;
    bool isFree;
    int life;
    QPixmap enemy2;
    int recored1;
    int recored2;
    int recored3;
    Bullet enemy2Bullet1[30];
    Bullet enemy2Bullet2[30];
    Bullet enemy2Bullet3[30];
    void shoot();
    void updatePosition();

};

#endif // ENEMYPLANE2_H
