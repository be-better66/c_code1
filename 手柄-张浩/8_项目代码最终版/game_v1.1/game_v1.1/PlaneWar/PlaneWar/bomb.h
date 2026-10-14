#ifndef BOMB_H
#define BOMB_H
#define BOMB_PATH ":/image/images/bomb-%1.png"
#include <QPixmap>
#include <QVector>
#include <QString>
class Bomb
{
public:
    Bomb();
    void updateInfo();
    int x;
    int y;
    QVector<QPixmap> bombPix;
    int recored;
    int index;
    bool isPlayde;
};

#endif // BOMB_H
