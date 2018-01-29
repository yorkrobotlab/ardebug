#ifndef BLUETOOTHCONFIG_H
#define BLUETOOTHCONFIG_H

#include <QMap>
#include <QString>
#include "bluetoothdevicelistitem.h"

class Bluetoothconfig
{
private:
    std::vector<BluetoothDeviceListItem*> deviceList;

public:
    Bluetoothconfig();
    ~Bluetoothconfig();

private:
    int readFile();
    int writeFile();
    void getActiveDevices();
};

#endif // BLUETOOTHCONFIG_H
