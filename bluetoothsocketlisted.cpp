#include "bluetoothsocketlisted.h"

BluetoothSocketListed::BluetoothSocketListed(QBluetoothAddress addr, int index, QObject *parent) : QObject(parent)
{
    this->index = index;
   socket = new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol);
   socket->connectToService(addr,0x0001,QIODevice::ReadWrite);
   connect(socket, SIGNAL(readyRead()), this, SLOT(socketReadyRead()));

   //connect(btSocket[i], SIGNAL(connected()), this, SLOT(connected()));
   //connect(btSocket[i], SIGNAL(disconnected()), this, SLOT(SocketDisconnected()));
   //connect(btSocket[i], SIGNAL(error(QBluetoothSocket::SocketError)), this, SLOT(SocketError(QBluetoothSocket::SocketError)));

}




void BluetoothSocketListed::socketReadyRead()
{
     emit readyRead_indexed(index);
}

BluetoothSocketListed::~ BluetoothSocketListed()
{

}

QByteArray BluetoothSocketListed::readLine()
{
    return socket->readLine();
}

bool BluetoothSocketListed::canReadLine()
{
    return socket->canReadLine();
}
