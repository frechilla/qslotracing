HEADERS += $$PWD/abstractserial.h \
    $$PWD/abstractserial_p.h \
    $$PWD/abstractserialengine.h \
    $$PWD/abstractserialengine_p.h \
    $$PWD/nativeserialengine.h \
    $$PWD/nativeserialengine_p.h \
    $$PWD/abstractserialnotifier.h \
    $$PWD/nativeserialnotifier.h

SOURCES += $$PWD/abstractserial.cpp \
    $$PWD/abstractserialengine.cpp \
    $$PWD/nativeserialengine.cpp \
    $$PWD/abstractserialnotifier.cpp \
    $$PWD/nativeserialengine_win.cpp \
    $$PWD/nativeserialnotifier_win.cpp
INCLUDEPATH += $$PWD
