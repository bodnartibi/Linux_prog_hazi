#-------------------------------------------------
#
# Project created by QtCreator 2015-04-24T21:09:34
#
#-------------------------------------------------

QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Client
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    clientMessages.cpp

HEADERS  += mainwindow.h \
        ../messages.h \
    clientMessages.h
