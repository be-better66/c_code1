#include "bullet.h"

Bullet::Bullet()
{
    bullet.load(":/image/images/mybullet.png");
    EnemyBullet.load(":/image/images/enemybullet.png");
    EnemyBullet2.load(":/image/images/bossbullet.png");
    lifesupply.load(":/image/images/lifesupply.png");
    bigbullet.load(":/image/images/bigbullet.png");
    speed = 10;
    isFree = true;
}

void Bullet::updatePosition(){
    if(isFree)
           return;
    y-=5;
    if(y<0)
        isFree = true;
}

void Bullet::EnemyUpdatePosition(){
    if(isFree)
        return;
    y+=5;
}

void Bullet::EnemyUpdatePositionLeft(){
    if(isFree)
        return;
    x-=2;
    y+=5;
}

void Bullet::EnemyUpdatePositionRight(){
    if(isFree)
        return;
    x+=2;
    y+=5;
}
