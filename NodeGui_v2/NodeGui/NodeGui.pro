#-------------------------------------------------
#
# Project created by QtCreator 2017-03-05T19:16:09
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = NodeGui
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp \
    NetworkClient.cpp

HEADERS  += MainWindow.h \
    NetworkClient.h

FORMS    += MainWindow.ui
