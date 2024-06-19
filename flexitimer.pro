TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        examples/basic_example.c \
        src/flexitimer.c

HEADERS += \
    include/flexitimer.h

INCLUDEPATH += $$PWD/include
