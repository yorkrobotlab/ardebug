#ifndef BLUETOOTHDEVICELISTITEM_H
#define BLUETOOTHDEVICELISTITEM_H

#include <QBluetoothAddress>

class BluetoothDeviceListItem
{
private:
    QBluetoothAddress bt_address;
    QString name;
    bool state;

public:
    QString getName();
    QString getBTAddress();
    BluetoothDeviceListItem(QString addr, QString name, bool state);
    BluetoothDeviceListItem(QBluetoothAddress addr, QString name, bool state);
    bool getState();
    void setState(bool state);

};

#endif // BLUETOOTHDEVICELISTITEM_H
