/* bluetoothsocketlisted.cpp
 *
 * holds a class that serves as an index bluetooth socket
 *
 * Charlotte Arndt Feb 2018
 */

#include "bluetoothsocketlisted.h"
#include <QDebug>

/* Constructor
 *
 */
BluetoothSocketListed::BluetoothSocketListed(QBluetoothAddress addr, int index, QObject *parent) : QObject(parent)
{
   this->index = index;
   socket = new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol);
   this->addr = addr;
   qDebug() << "new socket" <<this->addr;
}

/* socketReadyRead
 * Slot to handle the read status of the internal socket
 */
void BluetoothSocketListed::socketReadyRead()
{
     emit readyRead_indexed(index);
}

/* connectSocket
 * function to connect the internal socekt
 */
void BluetoothSocketListed::connectSocket()
{

    connect(socket, SIGNAL(readyRead()), this, SLOT(socketReadyRead()));
    connect(this, SIGNAL(connectInternalSocket()), this, SLOT(internalSocketConnector()));

    //signals for actual socket status for colour changing in UI
    connect(socket, SIGNAL(connected()), this, SLOT(internalSocketConnected()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(internalSocketDisconnected()));
    emit connectInternalSocket();
    //connect(btSocket[i], SIGNAL(error(QBluetoothSocket::SocketError)), this, SLOT(SocketError(QBluetoothSocket::SocketError)));
}

/* disconnectSocket
 * function to disconnect the internal socket
 */
void BluetoothSocketListed::disconnectSocket()
{
    connect(this, SIGNAL(disconnectInternalSocket()), this, SLOT(internalSocketDisconnector()));
    emit disconnectInternalSocket();
}


/* changeState
 * function connect or disconnect the socket, depending on its state
 */
void BluetoothSocketListed::changeState()
{
    if (socket->state() == QBluetoothSocket::ConnectedState)
        disconnectSocket();
    else if(socket->state()==QBluetoothSocket::UnconnectedState)
        connectSocket();
}

/* Destructor
 * frees memory
 */
BluetoothSocketListed::~ BluetoothSocketListed()
{
    delete socket;
}

/* readLine
 * read a line from the socket
 */
QByteArray BluetoothSocketListed::readLine()
{
    return socket->readLine();
}

/* canReadLine
 * returns the canReadLine value from the socket
 */
bool BluetoothSocketListed::canReadLine()
{
    return socket->canReadLine();
}

/* internalSocketConnector
 * Slot to connect internal socket
 */
void BluetoothSocketListed::internalSocketConnector()
{
    socket->connectToService(addr,0x0001,QIODevice::ReadWrite);
    qDebug() << "socket connected";
}

/* internalSocketConnector
 * Slot to disconnect internal socket
 */
void BluetoothSocketListed::internalSocketDisconnector()
{
    socket->disconnectFromService();
    qDebug() << "socket disconnected";
}


void BluetoothSocketListed::internalSocketDisconnected()
{
    emit socketDisconnected(this->index);
}

void BluetoothSocketListed::internalSocketConnected()
{
     emit socketConnected(this->index);
}
