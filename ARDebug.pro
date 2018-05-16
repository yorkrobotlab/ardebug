QT += core gui bluetooth opengl charts network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ardebug
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
    Application/Tracking/aruco.cpp \
    Application/Tracking/usbcamerathread.cpp \
    Application/Visualiser/vistext.cpp \
    Application/UI/bluetoothconfigdialog.cpp \
    Application/Tracking/cvbcamerathread.cpp

HEADERS  += Application/Core/mainwindow.h \
    Application/Core/util.h \
    Application/Core/settings.h \
    Application/Core/log.h \
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
    Application/Tracking/aruco.h \
    Application/Tracking/usbcamerathread.h \
    Application/Visualiser/vistext.h \
    Application/UI/bluetoothconfigdialog.h \
    Application/Tracking/cvbcamerathread.h \
    Application/Tracking/camerathread.h \
    Application/Core/defer.h \
    Application/Core/logging.h

FORMS += Application/UI/mainwindow.ui

CONFIG += link_pkgconfig \
    c++11

PKGCONFIG += opencv

exists("/opt/cvb/") {
    DEFINES += CVB_CAMERA_PRESENT

    LIBS += -L/opt/cvb/lib/ -lCVCImg
    LIBS += -L/opt/cvb/lib/ -lCVCDriver
    LIBS += -L/opt/cvb/lib/ -lCVCUtilities

    INCLUDEPATH += /opt/cvb/include
    DEPENDPATH += /opt/cvb/include
}
