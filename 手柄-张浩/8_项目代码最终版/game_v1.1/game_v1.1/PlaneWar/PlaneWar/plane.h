#ifndef PLANE_H
#define PLANE_H
#include <QPoint>
#include <QPixmap>
#include <bullet.h>
class Plane
{
public:
    //void setPosition(int x,int y);
    Bullet myBullet[30];
    Bullet BigBullet[5];
    Plane();
    void shoot();
    QPixmap myplane;
    int x;
    int y;
    int recored;
    int interval;
    int life;
    double skill;
    bool isPlayed;

};

#endif // PLANE_H
