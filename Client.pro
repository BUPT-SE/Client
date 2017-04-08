#-------------------------------------------------
#
# Project created by QtCreator 2017-03-27T16:00:44
#
#-------------------------------------------------

QT       += core gui
QT       += network


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Client
TEMPLATE = app


SOURCES += main.cpp\
        client.cpp \
    attribute.cpp \
    configdialog.cpp

HEADERS  += client.h \
    attribute.h \
    configdialog.h

FORMS    += client.ui
