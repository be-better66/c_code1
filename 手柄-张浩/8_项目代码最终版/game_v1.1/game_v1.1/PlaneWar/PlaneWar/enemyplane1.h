#ifndef ENEMYPLANE1_H
#define ENEMYPLANE1_H
#include <QPixmap>
#include <bullet.h>
#include <bomb.h>
class EnemyPlane1
{
public:
    Bomb bomb;
    EnemyPlane1();
    int x;
    int y;
    int speed;
    bool isFree;
    bool isDestroyed;
    QPixmap enemy1;
    int recored;
    Bullet enemy1Bullet[30];
    void shoot();
    void updatePosition();
};

#endif // ENEMYPLANE1_H
