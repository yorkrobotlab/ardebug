#ifndef BLUETOOTHDEVICELISTITEM_H
#define BLUETOOTHDEVICELISTITEM_H

#include <QBluetoothAddress>

class BluetoothDeviceListItem
{
private:
    QBluetoothAddress bt_adress;
    QString name;
    bool state;

public:
    QString getName();
    QString getBTAdress();
    BluetoothDeviceListItem(QString addr, QString name, bool state);
    BluetoothDeviceListItem(QBluetoothAddress addr, QString name, bool state);
    //bool operator<(const  BluetoothDeviceListItem & left , const  BluetoothDeviceListItem & right);
    bool getState();

};

#endif // BLUETOOTHDEVICELISTITEM_H
