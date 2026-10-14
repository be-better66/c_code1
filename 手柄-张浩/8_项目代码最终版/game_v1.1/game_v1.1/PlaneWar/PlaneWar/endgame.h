#ifndef ENDGAME_H
#define ENDGAME_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <easy.h>
#include <choose.h>
#include <QSerialPortInfo>
#include <QSerialPort>
#include <QVector>
#include "Windows.h"//这是引入Windows操作系统的API

class EndGame : public QWidget
{
    Q_OBJECT

public:
    explicit EndGame(QWidget *parent = 0);
    QPushButton *Return;
    QPushButton *Again;
    QLabel *Score;
    QLabel *Lose;
    EndGame(int x);
    ~EndGame();
    QSerialPort* m_serialPort;
    QVector<float> temp;
private slots:
    void serialconnect();
    void receiveInfo();
public slots:
    void ReturnClicked();
    void AgainClicked();
};

#endif // ENDGAME_H
