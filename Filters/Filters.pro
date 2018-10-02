#-------------------------------------------------
#
# Project created by QtCreator 2018-10-02T19:58:00
#
#-------------------------------------------------

QT       -= core gui

TARGET = Filters
TEMPLATE = lib

DEFINES += FILTERS_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

LIBS += -L/usr/local/lib \
-lopencv_core \
-lopencv_imgproc \

unix {
    target.path = /usr/lib
    INSTALLS += target
}

HEADERS += \
    filter.h \
    filterfactory.h \
    auxiliary_function.h \
    denoize_filter.h \
    irr_filter.h

SOURCES += \
    filterfactory.cpp \
    filter.cpp \
    auxiliary_function.cpp \
    denoize_filter.cpp \
    irr_filter.cpp
