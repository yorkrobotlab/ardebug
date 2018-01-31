#include <QFile>
#include "bluetoothconfig.h"
#include <QDebug>

Bluetoothconfig::Bluetoothconfig(QObject *parent) : QObject(parent)
{
    deviceList.reserve(10);
    readFile();

}


Bluetoothconfig::~Bluetoothconfig()
{
    for (size_t i = 0; i < deviceList.size(); i++) {
        delete deviceList[i];
    }
    deviceList.clear();
}

int Bluetoothconfig::writeFile()
{
    return 0;
}


int Bluetoothconfig::readFile()
{

    QFile btfile("bluetooth.config");
    if (btfile.exists())
    {
       qDebug() << "config file present";
    }
    else
    {
        qDebug() << "NO config file present";

    }

    //file string everything evaluation

    BluetoothDeviceListItem* newDevice = new BluetoothDeviceListItem("00:06:66:84:93:1E", "RNBT-931E", true);
    deviceList.push_back(newDevice);
     //qDebug() << "new device" <<newDevice->getBTAdress();
    return 1;
}

void Bluetoothconfig::getActiveDevices( std::vector<BluetoothDeviceListItem*> * activedeviceList)
{
    for (size_t i = 0; i < deviceList.size(); i++) {
        if(deviceList[i]->getState() == true)
        {
             activedeviceList->push_back(new BluetoothDeviceListItem(deviceList[i]->getBTAdress(), deviceList[i]->getName(), true));
             qDebug() << "active device" <<deviceList[i]->getBTAdress();
        }
    }
}
