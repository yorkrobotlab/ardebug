/* bluetoothdevicelistitem.cpp
 *
 * hold functions for the a class to handle
 * information about bluetooth devices
 *
 * Charlotte Arndt Feb 2018
 */

#include <QBluetoothAddress>
#include "bluetoothdevicelistitem.h"
#include <QDebug>

/* Constructor
 * initilises the variables
 */
BluetoothDeviceListItem::BluetoothDeviceListItem(QString addr, QString name, bool state)
{
    this->name = name;
    this->bt_address = QBluetoothAddress(addr);
    this->state = state;

}

/* Constructor
 * initilises the variables
 */
BluetoothDeviceListItem::BluetoothDeviceListItem(QBluetoothAddress addr, QString name, bool state)
{

    this->name = name;
    this->bt_address = QBluetoothAddress(addr);
    this->state = state;
}

/* getName
 * returns the name
 */
QString BluetoothDeviceListItem::getName()
{
    return name;
}

/* getBTAdress
 * returns the bluetooth adress
 */
QString BluetoothDeviceListItem::getBTAddress()
{
    return this->bt_address.toString();
}

/* getState
 * returns if the device is active or not
 */
bool BluetoothDeviceListItem::getState()
{

    return this->state;
}

void BluetoothDeviceListItem::setState(bool state)
{

    this->state = state;
}

