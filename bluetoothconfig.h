#ifndef BLUETOOTHCONFIG_H
#define BLUETOOTHCONFIG_H

#include <QMap>
#include <QString>
#include "bluetoothdevicelistitem.h"
#include <QObject>

class Bluetoothconfig : public QObject
{
    Q_OBJECT
private:
    std::vector<BluetoothDeviceListItem*> deviceList;
public:
    explicit Bluetoothconfig(QObject *parent = 0);
    ~Bluetoothconfig();
    void getActiveDevices( std::vector<BluetoothDeviceListItem*> * activedeviceList);

private:
    int readFile();
    int writeFile();

public slots:

signals:
    void deviceListChanged(void);
};

#endif //BLUETOOTHCONFIG_H
