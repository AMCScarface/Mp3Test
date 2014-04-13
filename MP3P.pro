#-------------------------------------------------
#
# Project created by QtCreator 2014-03-31T22:08:40
#
#-------------------------------------------------
QT += multimedia
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MP3P
TEMPLATE = app


SOURCES += main.cpp\
        musicplayer.cpp \
    radio.cpp

HEADERS  += musicplayer.h \
    radio.h
FORMS    += musicplayer.ui
