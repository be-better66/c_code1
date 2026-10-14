#ifndef RANKING_H
#define RANKING_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QString>
#include <QLabel>
#include <QPushButton>
#include <QSerialPortInfo>
#include <QSerialPort>
#include "Windows.h"//这是引入Windows操作系统的API

class Ranking : public QWidget
{
    Q_OBJECT

public:
    QSqlDatabase database;
    QLabel* Label1;
    QLabel* Label2;
    QLabel* Label3;
    QLabel* Label4;
    QLabel* Label5;
    QLabel* Label6;
    QPushButton* Return;
    QSerialPort* m_serialPort;
    QVector<float> temp;
    explicit Ranking(QWidget *parent = 0);
    void updateRanking(QString s,int Score);
    ~Ranking();
public slots:
    void ReturnClicked();
private slots:
    void serialconnect();
    void receiveInfo();
};

#endif // RANKING_H
