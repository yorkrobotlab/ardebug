#-------------------------------------------------
#
# Project created by QtCreator 2017-01-23T11:12:57
#
#-------------------------------------------------

QT       += core gui bluetooth opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ARDebug
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


SOURCES += Application/Core/main.cpp\
    Application/Core/mainwindow.cpp \
    Application/Core/util.cpp \
    Application/Core/settings.cpp \
    Application/Core/log.cpp \
    Application/DataModel/datamodel.cpp \
    Application/DataModel/robotdata.cpp \
    Application/Networking/Wifi/datathread.cpp \
    Application/Networking/Bluetooth/bluetoothdatathread.cpp \
    Application/Networking/Bluetooth/bluetoothdevicelistitem.cpp \
    Application/Networking/Bluetooth/bluetoothsocketlisted.cpp \
    Application/Networking/Bluetooth/bluetoothconfig.cpp \
    Application/Visualiser/visualiser.cpp \
    Application/Visualiser/visconfig.cpp \
    Application/Visualiser/visposition.cpp \
    Application/Visualiser/visname.cpp \
    Application/Visualiser/vispath.cpp \
    Application/UI/pathsettingsdialog.cpp \
    Application/UI/idsettingsdialog.cpp \
    Application/UI/namesettingsdialog.cpp \
    Application/UI/addidmappingdialog.cpp \
    Application/UI/robotinfodialog.cpp \
    Application/Tracking/aruco.cpp \
    Application/Tracking/usbcamerathread.cpp \
    Application/Visualiser/vistext.cpp \
    Application/UI/bluetoothconfigdialog.cpp


HEADERS  += Application/Core/mainwindow.h \
    Application/Core/util.h \
    Application/Core/settings.h \
    Application/Core/log.h \
    Application/Core/appconfig.h \
    Application/DataModel/datamodel.h \
    Application/DataModel/robotdata.h \
    Application/Networking/Wifi/datathread.h \
    Application/Networking/Bluetooth/bluetoothdevicelistitem.h \
    Application/Networking/Bluetooth/bluetoothsocketlisted.h \
    Application/Networking/Bluetooth/bluetoothdatathread.h \
    Application/Networking/Bluetooth/bluetoothconfig.h \
    Application/Visualiser/visualiser.h \
    Application/Visualiser/visconfig.h \
    Application/Visualiser/viselement.h \
    Application/Visualiser/visposition.h \
    Application/Visualiser/visname.h \
    Application/Visualiser/vispath.h \
    Application/UI/pathsettingsdialog.h \
    Application/UI/idsettingsdialog.h \
    Application/UI/namesettingsdialog.h \
    Application/UI/addidmappingdialog.h \
    Application/UI/robotinfodialog.h \
    Application/Tracking/aruco.h \
    Application/Tracking/usbcamerathread.h \
    Application/Visualiser/vistext.h \
    Application/UI/bluetoothconfigdialog.h


FORMS    += Application/UI/mainwindow.ui

CONFIG += link_pkgconfig \
    c++11

PKGCONFIG += opencv
