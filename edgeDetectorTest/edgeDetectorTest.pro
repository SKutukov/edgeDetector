QT += core
QT -= gui

CONFIG += c++11

TARGET = edgeDetectorTest
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    irr_tests.cpp \
    true_false_metric.cpp

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
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
-lopencv_highgui \
-lopencv_imgcodecs \
-lopencv_imgproc \
-lopencv_highgui \
-ltbb \

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../IRR_filter/release/ -lIRR_filter
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../IRR_filter/debug/ -lIRR_filter
else:unix: LIBS += -L$$OUT_PWD/../IRR_filter/ -lIRR_filter

INCLUDEPATH += $$PWD/../IRR_filter
DEPENDPATH += $$PWD/../IRR_filter

HEADERS += \
    true_false_metric.h
