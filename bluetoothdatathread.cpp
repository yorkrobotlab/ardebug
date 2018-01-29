/* bluetoothdatathread.cpp
 *
 * This class encapsulates a threaded Bluetooth connector. Data
 * that is received is sent to the main thread via the Qt signals
 * and slots system.
 *
 * (C) Charlotte Arndt Nov 2017
 */

#include "bluetoothdatathread.h"
#include <QString>
#include <QtBluetooth/QBluetoothSocket>
#include <QtBluetooth/QBluetoothServiceInfo>
#include <QFile>


BluetoothDataThread::~BluetoothDataThread(){
    stop();
}

BluetoothDataThread::BluetoothDataThread(){

    /*
    for (int i = 0; i< NUMBER_OF_BT_SOCKET; i++)
    {
        btSocket[i] = 0;
    }*/
}

void BluetoothDataThread::stop(){
    /*
     qDebug() << "Delete socket";
     for (int i = 0; i< NUMBER_OF_BT_SOCKET; i++)
     {
        if(btSocket[i] != 0)
        {
            delete btSocket[i];
            btSocket[i] = 0;
        }
     }*/

}


void BluetoothDataThread::connected()
{
 //   qDebug()<<btSocket[0]->peerName();
}


/* readSocket
 * slot function, reads data from bt sockets when present
 */
void BluetoothDataThread::readSocket(int index)
{

    while (btSocket[index]->canReadLine()) {
         qDebug() << "readline from socket";
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
        if (btSocket[i] != 0)
        {
            //btSocket[i] = new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol);
            btSocket[i] = new BluetoothSocketListed(addr, i);

            qDebug() << "socket connected";

            connect(btSocket[i], SIGNAL(readyRead_indexed(int)), this, SLOT(readSocket(int)));
            //connect(btSocket[i], SIGNAL(readyRead(int)), this, SLOT(readSocket(int)));
            //connect(btSocket[i], SIGNAL(connected()), this, SLOT(connected()));
            //connect(btSocket[i], SIGNAL(disconnected()), this, SLOT(SocketDisconnected()));
            //connect(btSocket[i], SIGNAL(error(QBluetoothSocket::SocketError)), this, SLOT(SocketError(QBluetoothSocket::SocketError)));
            return 1;
        }
    }
    return 0;

}


void BluetoothDataThread::SocketDisconnected()
{

}

