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


BluetoothDataThread::~BluetoothDataThread(){
    stop();
}

BluetoothDataThread::BluetoothDataThread(){
    for (int i = 0; i< 2; i++)
    {
        btSocket[i] = 0;
    }
}

void BluetoothDataThread::stop(){
     qDebug() << "Delete socket";
     for (int i = 0; i< 2; i++)
     {
        delete btSocket[i];
        btSocket[i] = 0;
     }

}


void BluetoothDataThread::connected()
{
    qDebug()<<btSocket[0]->peerName();
}


/* openSocket
 * Opens a RFCOMM bluetooth socket and begins listening for data.
 */
void BluetoothDataThread::readSocket()
{
    qDebug() << "form socket";
    for (int i = 0; i< 2; i++)
    {
        if (!btSocket[i])
          continue;

        while (btSocket[i]->canReadLine()) {
             qDebug() << "readline form socket";
            QByteArray line = btSocket[i]->readLine();
            emit dataFromThread(QString::fromUtf8(line.constData(), line.length()));
        }
    }
}


/* openSocket
 * Opens a RFCOMM bluetooth socket and begins listening for data.
 */
void BluetoothDataThread::openSocket()
{
    for (int i = 0; i< 2; i++)
    {
        btSocket[i] = new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol);

        qDebug() << "Create socket";
        if (i == 0)
            btSocket[i]->connectToService(QBluetoothAddress(dest),0x0001,QIODevice::ReadWrite);
        else
             btSocket[i]->connectToService(QBluetoothAddress(dest2),0x0001,QIODevice::ReadWrite);
        qDebug() << "socket connected";

        connect(btSocket[i], SIGNAL(readyRead()), this, SLOT(readSocket()));
        connect(btSocket[i], SIGNAL(connected()), this, SLOT(connected()));
    }
}
