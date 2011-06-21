#-------------------------------------------------
#
# Project created by QtCreator 2010-12-21T16:02:47
#
#-------------------------------------------------

QT       += core gui

TARGET = QSlotRacing
TEMPLATE = app

CONFIG += qwt
INCLUDEPATH += "$$OUT_PWD/include/qwt"
LIBS += "$$OUT_PWD/lib/libqwt.a"
LIBS += "$$OUT_PWD/lib/libqwtmathml.a"

include(src/src.pri)
include(src/qserialdevice/qserialdevice.pri)

HEADERS +=

SOURCES +=

FORMS +=
