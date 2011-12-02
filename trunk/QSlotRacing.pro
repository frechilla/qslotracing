#-------------------------------------------------
#
# Project created by QtCreator 2010-12-21T16:02:47
#
#-------------------------------------------------

QT       += core gui phonon

TARGET = QSlotRacing
TEMPLATE = app

CONFIG += qwt

INCLUDEPATH += "$$PWD/3rdParty/qserialdevice/include/qserialdevice"
INCLUDEPATH += "$$PWD/3rdParty/qserialdevice/include/qserialdeviceenumerator"
INCLUDEPATH += "$$PWD/3rdParty/qwt/include"

win32 {
    # LIBS += -lsetupapi -luuid -ladvapi32  ## libraries used to compile qserialdevice
    LIBS += "$$PWD/3rdParty/qserialdevice/i386-win32/lib/libqserialdevice.a"
    LIBS += "$$PWD/3rdParty/qwt/i386-win32/lib/libqwt.a"
    LIBS += "$$PWD/3rdParty/qwt/i386-win32/lib/libqwtmathml.a"
}
unix:!macx {
    #LIBS += -ludev  ## libraries used to compile qserialdevice
    LIBS += "$$PWD/3rdParty/qserialdevice/i386-linux/lib/libqserialdevice.a"
    LIBS += "$$PWD/3rdParty/qwt/i386-linux/lib/libqwt.a"
    LIBS += "$$PWD/3rdParty/qwt/i386-linux/lib/libqwtmathml.a"
}
macx {
    LIBS += -framework IOKit -framework CoreFoundation
    #TODO put path to MacOS X library here
}


include(src/src.pri)

HEADERS +=

SOURCES +=

FORMS +=
