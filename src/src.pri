SOURCES += src/main.cpp\
    src/mainwindow.cpp \
    src/scxmsgfactory.cpp \
    src/snifferfileascii.cpp \
    src/qslotracingmsg.cpp \
    src/scxproto.cpp \
    src/configdialog.cpp \
    src/recorderbinary.cpp \
    src/snifferserial.cpp \
    src/serialmonitor.cpp \
    src/controllerdlg.cpp \
    src/statsdialog.cpp \
    src/qchartframe.cpp

HEADERS += src/mainwindow.h \
    src/qslotracingmsg.h \
    src/scxmsgfactory.h \
    src/snifferfileascii.h \
    src/qslotracingmsgfactory.h \
    src/scxproto.h \
    src/configdialog.h \
    src/qslotracingevent.h \
    src/recorderbinary.h \
    src/qserialdevice_global.h \
    src/snifferserial.h \
    src/serialmonitor.h \
    src/controllerdlg.h \
    src/statsdialog.h   \
    src/atomic_ops.h \
    src/qslotracingstatscounter.h \
    src/qchartframe.h

FORMS   += src/mainwindow.ui \
    src/configdialog.ui \
    src/serialmonitor.ui \
    src/controllerdlg.ui \
    src/statsdialog.ui

RESOURCES += \
    src/pics/tool.qrc
