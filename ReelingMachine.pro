#-------------------------------------------------
#
# Project created by QtCreator 2013-07-07T10:00:29
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RMachine
TEMPLATE = app

SOURCES += main.cpp\
        widget.cpp

HEADERS  += widget.h


include(qextserialport/qextserialport.pri)

RC_FILE += theAppIcon.rc

DEFINES +=  QT_NO_DEBUG_OUTPUT

