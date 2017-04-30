#-------------------------------------------------
#
# Project created by QtCreator 2017-03-10T22:53:59
#
#-------------------------------------------------

QT       += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = VRKMsso
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    networking.cpp \
    devicemanager.cpp \
    messageworker.cpp \
    databaseworker.cpp

HEADERS  += mainwindow.h \
    networking.h \
    devicemanager.h \
    messageworker.h \
    databaseworker.h

FORMS    += mainwindow.ui \
    devicemanager.ui

RESOURCES += \
    resources.qrc
