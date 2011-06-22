#-------------------------------------------------
#
# Project created by QtCreator 2010-12-21T16:02:47
#
#-------------------------------------------------

QT       += core gui

TARGET = QSlotRacing
TEMPLATE = app

CONFIG += qwt
INCLUDEPATH += "$$PWD/3rdParty/qwt/include"
LIBS += "$$PWD/3rdParty/qwt/i386-win32/lib/libqwt.a"
LIBS += "$$PWD/3rdParty/qwt/i386-win32/lib/libqwtmathml.a"

INCLUDEPATH += "$$PWD/3rdParty/qserialdevice/include/qserialdevice"
INCLUDEPATH += "$$PWD/3rdParty/qserialdevice/include/qserialdeviceenumerator"
LIBS += "$$PWD/3rdParty/qserialdevice/i386-win32/lib/libqserialdevice.a"

include(src/src.pri)

HEADERS +=

SOURCES +=

FORMS +=
