#-------------------------------------------------
#
# Project created by QtCreator 2016-03-20T14:15:33
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtToPythonTest
TEMPLATE = app

# add python path
#INCLUDEPATH += C:\Python35\include\

SOURCES += main.cpp\
        mainwindow.cpp \
    pythonwrapper.cpp \
    exposedclasstest1.cpp \
    pythonwrapper_qtpart.cpp

HEADERS  += mainwindow.h \
    pythonwrapper.h \
    exposedclasstest1.h \
    pythonwrapper_qtpart.h

FORMS    += mainwindow.ui

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../../../Python34/libs/ -lpython34
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../../../Python34/libs/ -lpython34
else:unix:!macx: LIBS += -L$$PWD/../../../../../../../Python34/libs/ -lpython34

INCLUDEPATH += $$PWD/../../../../../../../Python34/include
DEPENDPATH += $$PWD/../../../../../../../Python34/libs
