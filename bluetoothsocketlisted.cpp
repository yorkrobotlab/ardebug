#include "bluetoothsocketlisted.h"
#include <QDebug>

BluetoothSocketListed::BluetoothSocketListed(QBluetoothAddress addr, int index, QObject *parent) : QObject(parent)
{
   this->index = index;
   socket = new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol);
   this->addr = addr;
   qDebug() << "new socket" <<this->addr;



}

void BluetoothSocketListed::socketReadyRead()
{
     emit readyRead_indexed(index);
}

void BluetoothSocketListed::connectSocket()
{

    connect(socket, SIGNAL(readyRead()), this, SLOT(socketReadyRead()));
    connect(this, SIGNAL(connectInternalSocket()), this, SLOT(internalSocketConnector()));
    qDebug() << "vielleicht connected?";
    emit connectInternalSocket();
    //connect(btSocket[i], SIGNAL(connected()), this, SLOT(connected()));
    //connect(btSocket[i], SIGNAL(disconnected()), this, SLOT(SocketDisconnected()));
    //connect(btSocket[i], SIGNAL(error(QBluetoothSocket::SocketError)), this, SLOT(SocketError(QBluetoothSocket::SocketError)));
}

BluetoothSocketListed::~ BluetoothSocketListed()
{
    delete socket;
}

QByteArray BluetoothSocketListed::readLine()
{
    return socket->readLine();
}

bool BluetoothSocketListed::canReadLine()
{
    return socket->canReadLine();
}

void BluetoothSocketListed::internalSocketConnector()
{
    socket->connectToService(addr,0x0001,QIODevice::ReadWrite);
    qDebug() << "connect slot?";
}
