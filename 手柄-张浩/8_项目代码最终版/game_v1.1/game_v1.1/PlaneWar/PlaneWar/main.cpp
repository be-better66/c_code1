#include "widget.h"
#include <QApplication>
#include <QSplashScreen>
#include <QPixmap>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSplashScreen *Splash = new QSplashScreen;
    Splash->setPixmap(QPixmap(":/image/images/welcome.png"));
    Splash->show();
    for(int i=0;i<3000;i++)
        Splash->repaint();
    Widget w;
    w.show();
    return a.exec();
}
