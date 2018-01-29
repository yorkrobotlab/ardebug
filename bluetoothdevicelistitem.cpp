#include <QBluetoothAddress>
#include "bluetoothdevicelistitem.h"


BluetoothDeviceListItem::BluetoothDeviceListItem(QString addr, QString name, bool state)
{
    this->name = name;
    this->bt_adress = QBluetoothAddress(addr);
    this->state = state;

}

BluetoothDeviceListItem::BluetoothDeviceListItem(QBluetoothAddress addr, QString name, bool state)
{

    this->name = name;
    this->bt_adress = QBluetoothAddress(addr);
    this->state = state;

}

QString BluetoothDeviceListItem::getName()
{

    return name;
}

QString BluetoothDeviceListItem::getBTAdress()
{
    return this->bt_adress.toString();
}

bool BluetoothDeviceListItem::getState()
{

    return this->state;
}

/*bool operator<(const  BluetoothDeviceListItem & left , const  BluetoothDeviceListItem & right)
{
    return left.Name()< right.getName();

}*/
