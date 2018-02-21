/* bluetoothconfig.cpp
 *
 * functions to configure the bluetooth connections.
 * a config file is used to store known devices
 * a scanner and modification functions are to be added
 *
 * Charlotte Arndt Feb 2018
 */

#include <QFile>
#include "bluetoothconfig.h"
#include <QDebug>


/* Constructor
 * initialisation
 */
Bluetoothconfig::Bluetoothconfig(QObject *parent) : QObject(parent)
{    
    deviceList.reserve(10);

    // Create a  model for the list of active bluetooth devices
    deviceListModel = new QStandardItemModel();


    readFile();

}
/* Deconstructor
 * frees memory where needed
 *
 */
Bluetoothconfig::~Bluetoothconfig()
{
    for (size_t i = 0; i < deviceList.size(); i++) {
        delete deviceList[i];
    }
    deviceList.clear();
}

/* writeFile
 * function to write the modified configfile
 *
 */
int Bluetoothconfig::writeFile()
{
    return 0;
}

/* readFile
 * reads the config file and processes its content
 *
 */
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
        QString address;
        QString name;

        //remove extra whitespaces
        line = line.simplified();
        if (!line.isEmpty())
        {
            if (line.at(0) == '#')
            {
                state = false;
                int divider = line.indexOf(" ");
                address = line.mid(1,divider-1);
                name = line.mid(divider+1);

            }
            else
            {
                state = true;
                int divider = line.indexOf(" ");
                address = line.mid(0,divider);
                name = line.mid(divider+1);

            }
            BluetoothDeviceListItem* newDevice = new BluetoothDeviceListItem(address, name, state);
            deviceList.push_back(newDevice);
        }
    }
    return 1;
}


/* getActiveDevices
 * adds active devices to the given vector *
 */
void Bluetoothconfig::getActiveDevices( std::vector<BluetoothDeviceListItem*> * activedeviceList)
{
    for (size_t i = 0; i < deviceList.size(); i++) {
        if(deviceList[i]->getState() == true)
        {
             activedeviceList->push_back(new BluetoothDeviceListItem(deviceList[i]->getBTAddress(), deviceList[i]->getName(), true));
             qDebug() << "active device" <<deviceList[i]->getBTAddress();
        }
    }
}

/* getActiveDeviceList
 * returns a Listmodel with current devices to be used for GUI
 *
 *
 */
QStandardItemModel* Bluetoothconfig::getActiveDeviceList(void) {
    int row = 0;

    for(size_t i = 0; i < deviceList.size(); i++) {

        BluetoothDeviceListItem* item = (BluetoothDeviceListItem*)deviceList.at(i);
        if (item->getState() == true)
        {
            QStandardItem *list_item = new QStandardItem(QString("%0 : %1").arg(item->getName()).arg(item->getBTAddress()));
            deviceListModel->setItem(row, 0, list_item),
            row++;
        }
    }



    return deviceListModel;
}


/* setItemColour
 * sets the colour of a model item
 */
 void Bluetoothconfig::setItemColour(int index,  Qt::GlobalColor colour){


    QModelIndex vindex = deviceListModel->index(index,0);
    deviceListModel->setData(vindex, QBrush(colour), Qt::BackgroundRole);

 }


 /* getDeviceList
  * returns a copy of the current devicelist
  */
 std::vector<BluetoothDeviceListItem*> Bluetoothconfig::getDeviceList(){


     return deviceList;
 }


 /* setDeviveList
  * sets a new device list and saves the updates to the config file
  */
 void Bluetoothconfig::setDeviceList(std::vector<BluetoothDeviceListItem*> newDeviceList)
 {
     writeFile();

     emit deviceListChanged();
 }
