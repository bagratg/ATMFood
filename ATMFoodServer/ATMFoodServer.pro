QT += core
QT += network
QT += sql
QT -= gui

CONFIG += c++11

TARGET = ATMFoodServer
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    connectdb.cpp \
    networkmanager.cpp \
    controller.cpp \
    sesioncontroller.cpp \
    session.cpp

HEADERS += \
    connectdb.h \
    networkmanager.h \
    controller.h \
    requestresponse.h \
    sesioncontroller.h \
    session.h \
    information.h
