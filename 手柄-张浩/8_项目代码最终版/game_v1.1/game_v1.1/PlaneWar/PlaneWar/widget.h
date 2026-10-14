#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QPainter>
#include <QPixmap>
#include <QImage>
#include <Qpaintevent>
#include <choose.h>
#include <QLabel>
#include <ranking.h>
#include <helpwidget.h>
#include <QApplication>
#include <QSerialPortInfo>
#include <QSerialPort>
#include <QVector>
#include "Windows.h"//这是引入Windows操作系统的API

class Widget : public QWidget
{
    Q_OBJECT
public slots:
    void startClick();
    void quitClick();
    void HelpClick();

public:
    Widget(QWidget *parent = 0);
    ~Widget();
private:
    QPushButton *startGame;
    QPushButton *quit;
    QImage* background;
    QLabel* label;
    QPushButton* Help;
    //paintEvent(QPaintEvent *event);
    QSerialPort* m_serialPort;
    QVector<float> temp;
private slots:
    void serialconnect();
    void receiveInfo();
};

#endif // WIDGET_H
