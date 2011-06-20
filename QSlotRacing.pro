#-------------------------------------------------
#
# Project created by QtCreator 2010-12-21T16:02:47
#
#-------------------------------------------------

QT       += core gui

TARGET = QSlotRacing
TEMPLATE = app

CONFIG += qwt
INCLUDEPATH += "C:/Qwt-6.0.1/include"
LIBS += "C:/Qwt-6.0.1/lib/libqwt.a"
LIBS += "C:/Qwt-6.0.1/lib/libqwtmathml.a"

include(src/src.pri)
include(src/qserialdevice/qserialdevice.pri)

HEADERS +=

SOURCES +=

FORMS +=
