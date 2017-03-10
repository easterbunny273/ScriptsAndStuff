#-------------------------------------------------
#
# Project created by QtCreator 2016-11-13T14:29:39
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = NodeGui
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    BasicNodes.cpp \
    Node.cpp \
    NodeNetwork.cpp \
    renderarea.cpp \
    ParameterItem.cpp \
    NodeItem.cpp \
    GraphicalNodeNetwork.cpp

HEADERS  += mainwindow.h \
    BasicNodes.h \
    EvaluationContext.h \
    IVariable.h \
    Node.h \
    NodeNetwork.h \
    renderarea.h \
    ParameterItem.h \
    NodeItem.h \
    GraphicalNodeNetwork.h

FORMS    += mainwindow.ui
