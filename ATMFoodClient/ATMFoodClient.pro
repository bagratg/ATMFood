#-------------------------------------------------
#
# Project created by QtCreator 2018-05-01T13:51:22
#
#-------------------------------------------------

QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ATMFoodClient
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    networkmanager.cpp \
    controller.cpp \
    userwindow.cpp \
    foodmodel.cpp \
    cartmodel.cpp \
    ordersmodel.cpp \
    treeitem.cpp \
    cartdialog.cpp \
    usermodel.cpp \
    settingwindow.cpp

HEADERS  += mainwindow.h \
    requestresponse.h \
    networkmanager.h \
    controller.h \
    userwindow.h \
    loginInformation.h \
    foodinformation.h \
    foodmodel.h \
    cartinformation.h \
    cartmodel.h \
    ordersmodel.h \
    order.h \
    treeitem.h \
    cartdialog.h \
    optionsinformation.h \
    usermodel.h \
    userinformation.h \
    settingwindow.h

FORMS    += mainwindow.ui \
    userwindow.ui \
    cartdialog.ui \
    settingwindow.ui

RESOURCES += \
    res.qrc

RC_FILE = appIcon.rc
