#include <QFile>
#include "bluetoothconfig.h"
#include <QDebug>

Bluetoothconfig::Bluetoothconfig(QObject *parent) : QObject(parent)
{    
    deviceList.reserve(10);

    // Create a  model for the list of active bluetooth devices
    deviceListModel = new QStringListModel();


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
    btfile.open(QIODevice::ReadWrite);

    while(!btfile.atEnd())
    {
        QByteArray line = btfile.readLine();
        bool state;
        QString adress;
        QString name;

        //remove extra whitespaces
        line = line.simplified();
        if (!line.isEmpty())
        {
            if (line.at(0) == '#')
            {
                state = false;
                int divider = line.indexOf(" ");
                adress = line.mid(1,divider-1);
                name = line.mid(divider+1);

            }
            else
            {
                state = true;
                int divider = line.indexOf(" ");
                adress = line.mid(0,divider);
                name = line.mid(divider+1);

            }
            BluetoothDeviceListItem* newDevice = new BluetoothDeviceListItem(adress, name, state);
            deviceList.push_back(newDevice);
        }
    }
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

QStringListModel* Bluetoothconfig::getActiveDeviceList(void) {
    QStringList list;
    qDebug() << "active device list";
    for(size_t i = 0; i < deviceList.size(); i++) {

        BluetoothDeviceListItem* item = (BluetoothDeviceListItem*)deviceList.at(i);
        if (item->getState() == true)
        {

            QString str(item->getName()+ ": " + item->getBTAdress());
            list.append(str);
        }
    }

    // Return the list model
    deviceListModel->setStringList(list);
    qDebug() << "active device list end" << list.size()<<deviceList.size();
    return deviceListModel;
}
