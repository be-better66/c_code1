#ifndef BULLET_H
#define BULLET_H
#include <QString>
#include <QPixmap>
class Bullet
{
public:
    Bullet();
    QPixmap bullet;
    QPixmap EnemyBullet;
    QPixmap EnemyBullet2;
    QPixmap lifesupply;
    QPixmap bigbullet;
    int power;
    int speed;
    int x;
    int y;
    void updatePosition();
    void EnemyUpdatePosition();
    void EnemyUpdatePositionLeft();
    void EnemyUpdatePositionRight();
    bool isFree;
};

#endif // BULLET_H
