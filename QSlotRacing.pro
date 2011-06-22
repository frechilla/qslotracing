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

INCLUDEPATH += "$$OUT_PWD/include/qserialdevice/qserialdevice"
INCLUDEPATH += "$$OUT_PWD/include/qserialdevice/qserialdeviceenumerator"
LIBS += "$$OUT_PWD/lib/libqserialdevice.a"

include(src/src.pri)

HEADERS +=

SOURCES +=

FORMS +=
