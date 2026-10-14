#-------------------------------------------------
#
# Project created by QtCreator 2020-07-07T16:34:35
#
#-------------------------------------------------

QT       += core gui serialport
QT       += sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PlaneWar
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        widget.cpp \
    choose.cpp \
    bullet.cpp \
    plane.cpp \
    easy.cpp \
    enemyplane1.cpp \
    enemyplane2.cpp \
    bomb.cpp \
    endgame.cpp \
    wingame.cpp \
    easy2.cpp \
    hard.cpp \
    hard2.cpp \
    ranking.cpp \
    helpwidget.cpp

HEADERS  += widget.h \
    choose.h \
    bullet.h \
    plane.h \
    easy.h \
    enemyplane1.h \
    enemyplane2.h \
    bomb.h \
    endgame.h \
    wingame.h \
    easy2.h \
    hard.h \
    hard2.h \
    ranking.h \
    helpwidget.h

RESOURCES += \
    resource.qrc

FORMS +=

 QT += multimedia

RC_ICONS = warofplanesicon.ico
