#include "enemyplane1.h"

EnemyPlane1::EnemyPlane1()
{
    enemy1.load(":/image/images/enemyplane.png");
    x = 0;
    y = 0;
    isFree = true;
    isDestroyed = false;
    speed = 1.5;
    recored = 0;
}

void EnemyPlane1::updatePosition(){
    if(isFree)
        return;
    y+=speed;
    if(y>600)
        isDestroyed = true;
}

void EnemyPlane1::shoot(){
    recored++;
    if(recored<50)
        return;
    recored = 0;
    for(int i=0;i<30;i++){
        if(enemy1Bullet[i].isFree){
            enemy1Bullet[i].x = x+20;
            enemy1Bullet[i].y = y+40;
            enemy1Bullet[i].isFree = false;
            break;
        }
    }
}
