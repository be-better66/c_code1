#ifndef WINGAME_H
#define WINGAME_H

#include <QWidget>
#include "easy.h"
#include "easy2.h"
#include "choose.h"
#include "hard.h"
#include "hard2.h"
#include <QPushButton>
#include <QSerialPortInfo>
#include <QSerialPort>
#include <QVector>
#include "Windows.h"//这是引入Windows操作系统的API

class WinGame : public QWidget
{
    Q_OBJECT

public:
    explicit WinGame(QWidget *parent = 0);
    int next;
    QLabel *Score;
    QPushButton *Return;
    QPushButton *Next;
    QLabel *Win;
    QSerialPort* m_serialPort;
    QVector<float> temp;
    WinGame(int x,int y);
    WinGame(int x);
    ~WinGame();
public slots:
    void NextClicked();
    void ReturnClicked();
private slots:
    void serialconnect();
    void receiveInfo();
};

#endif // WINGAME_H
