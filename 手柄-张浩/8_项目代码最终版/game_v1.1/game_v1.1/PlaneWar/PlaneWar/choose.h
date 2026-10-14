#ifndef CHOOSE_H
#define CHOOSE_H
//#include <QDialog>
#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include "widget.h"
#include "easy.h"
#include "easy2.h"
#include "hard.h"
#include "hard2.h"
#include <QSound>
#include <QSerialPortInfo>
#include <QSerialPort>
#include <QVector>
#include "Windows.h"//这是引入Windows操作系统的API

#define BOMB_SOUND ":/image/images/bomb.wav"
//class Choose: public QDialog
class Choose: public QWidget
{
    Q_OBJECT
public:
    Choose(QWidget *parent = 0);
    QPushButton* RankButton;
    QPushButton* easy2;
    QPushButton* easy;
    QPushButton* hard;
    QPushButton* hard2;
    QLabel *label1;
    QLabel *label2;
    QPushButton *Recored;
    QPushButton *Quit;
    QSerialPort* m_serialPort;
    QVector<float> temp;
    int delay_count;
    ~Choose();
public slots:
    void easyClicked();
    void easy2Clicked();
    void hardClicked();
    void hard2Clicked();
    void RankClicked();
    void QuitClicked();

private slots:
    void serialconnect();
    void receiveInfo();
};

#endif // CHOOSE_H
