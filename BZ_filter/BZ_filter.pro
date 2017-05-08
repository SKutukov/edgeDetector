#-------------------------------------------------
#
# Project created by QtCreator 2017-05-08T13:37:45
#
#-------------------------------------------------

QT       -= core gui

TARGET = BZ_filter
TEMPLATE = lib

DEFINES += BZ_FILTER_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

include(BZ_filter.pri)

unix {
    target.path = /usr/lib
    INSTALLS += target
}

LIBS += -L/usr/local/lib \
-lopencv_core \
-lopencv_imgproc \
-lopencv_imgcodecs \
