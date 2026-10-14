#include "plane.h"
#include <cstdlib>
#include <iostream>
#include <fstream>

Plane::Plane()
{
    myplane.load(":/image/images/myplane.png");
    x = (500-myplane.width())*0.5;
    y = 600-myplane.height();
    recored = 0;
    life = 10;
    isPlayed = false;
    skill = 20;
    interval = 30;
}

void Plane::shoot(){
    recored++;
    if(recored<interval)
        return;
    if(interval==15)
        skill-=0.1;
    if(skill==0)
        interval = 30;
    recored = 0;
    for(int i = 0;i<30;i++){
        if(myBullet[i].isFree){
            myBullet[i].x = x+40;
            myBullet[i].y = y-10;
            myBullet[i].isFree = false;
            break;
        }
    }
}
