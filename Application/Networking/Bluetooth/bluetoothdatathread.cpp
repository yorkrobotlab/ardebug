/* bluetoothdatathread.cpp
 *
 * This class encapsulates a threaded Bluetooth connector. Data
 * that is received is sent to the main thread via the Qt signals
 * and slots system.
 *
 * (C) Charlotte Arndt Nov 2017
 */

#include <QString>
#include <QtBluetooth/QBluetoothSocket>
#include <QtBluetooth/QBluetoothServiceInfo>
#include <QFile>
#include "bluetoothdatathread.h"

/* Destructor
 */
BluetoothDataThread::~BluetoothDataThread(){
    stop();
}

/* Constructor
 * sets up the initial settings
 */
BluetoothDataThread::BluetoothDataThread(Bluetoothconfig * btConfig){
    this->btConfig = btConfig;
    connect(this->btConfig, SIGNAL(deviceListChanged()),this,SLOT(updateSocketList()));

    for (int i = 0; i< NUMBER_OF_BT_SOCKET; i++)
    {
        btSocket[i] = 0;
    }

    //Load first list from btConfig

    std::vector<BluetoothDeviceListItem*> activeDeviceList;
    activeDeviceList.reserve(NUMBER_OF_BT_SOCKET);
    btConfig->getActiveDevices(&activeDeviceList);

    for (size_t i = 0; i < activeDeviceList.size(); i++) {

        //update sockets somehow
        openSocket(QBluetoothAddress(activeDeviceList[i]->getBTAdress()));
        qDebug() << "open socket "<<activeDeviceList[i]->getBTAdress();
        delete activeDeviceList[i];

    }
    activeDeviceList.clear();
     qDebug() << "btDatathread constructed";
}

/*  stop
 *  destructor method
 */
void BluetoothDataThread::stop(){

    qDebug() << "Delete sockets";
    for (int i = 0; i< NUMBER_OF_BT_SOCKET; i++)
    {
        if(btSocket[i] != 0)
        {
            delete btSocket[i];
            btSocket[i] = 0;
            qDebug() << "socket " <<i <<" deleted";
        }
    }
}


/* readSocket
 * slot function, reads data from bt sockets when present
 */
void BluetoothDataThread::readSocket(int index)
{

    while (btSocket[index]->canReadLine()) {
        //qDebug() << "readline from socket";
        QByteArray line = btSocket[index]->readLine();
        emit dataFromThread(QString::fromUtf8(line.constData(), line.length()));
    }
}


/* openSocket
 * Opens a RFCOMM bluetooth socket and begins listening for data.
 */
int BluetoothDataThread::openSocket(QBluetoothAddress addr)
{
    for (int i = 0; i< NUMBER_OF_BT_SOCKET; i++)
    {
        if (btSocket[i] == 0)
        {
            btSocket[i] = new BluetoothSocketListed(addr, i);
            connect(btSocket[i], SIGNAL(readyRead_indexed(int)), this, SLOT(readSocket(int)));
            //qDebug() << "opened";
            //connect(btSocket[i], SIGNAL(error(QBluetoothSocket::SocketError)), this, SLOT(SocketError(QBluetoothSocket::SocketError)));
            return 1;
        }
        else
        {
            //qDebug() << "socket already in use";
        }
    }
    return 0;

}

/* changeSocket
 * Slot to handle ui signal
 * connect/disconnect a socket
 */
void BluetoothDataThread::changeSocket(int index)
{
    if (btSocket[index] != 0)
    {
        btSocket[index]->changeState();

        qDebug() << "socket "<< index<< " connected";

    }




}


/* connectAllSockets
 * Slot to handle ui signal
 * connect all sockets
 */
void BluetoothDataThread::connectAllSockets()
{
    for (int i = 0; i< NUMBER_OF_BT_SOCKET; i++)
    {
        if (btSocket[i] != 0)
        {
            btSocket[i]->connectSocket();
        }      
    }
}


/* disconnectAllSockets
 * Slot to handle ui signal
 * disconnect all sockets
 */
void BluetoothDataThread::disconnectAllSockets()
{
    for (int i = 0; i< NUMBER_OF_BT_SOCKET; i++)
    {
        if (btSocket[i] != 0)
        {
            btSocket[i]->disconnectSocket();
        }
    }
}

/* SocketDisconnected
 * Slot to handle signal from sockets
 */
void BluetoothDataThread::SocketDisconnected()
{


}

/* updateSocketList
 * Slot to handle signal config when socket list modified
 */
void BluetoothDataThread::updateSocketList()
{
     std::vector<BluetoothDeviceListItem*> activeDeviceList;
     activeDeviceList.reserve(NUMBER_OF_BT_SOCKET);

    for (size_t i = 0; i < activeDeviceList.size(); i++) {

        //update sockets somehow

        delete activeDeviceList[i];
    }
    activeDeviceList.clear();
}

