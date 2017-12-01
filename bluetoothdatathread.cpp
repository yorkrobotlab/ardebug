/* bluetoothdatathread.cpp
 *
 * This class encapsulates a threaded Bluetooth connector. Data
 * that is received is sent to the main thread via the Qt signals
 * and slots system.
 *
 * (C) Charlotte Arndt Nov 2017
 */

#include "bluetoothdatathread.h"
//#include "sys/socket.h"
//#include <unistd.h>



#include <QString>
#include <QtBluetooth/QBluetoothSocket>
#include <QtBluetooth/QBluetoothServiceInfo>

/* listenForPacket
 * Listens for data on the bluetooth socket. Blocking.
 */
void BluetoothDataThread::listenForPacket(void) {
    /*
    int  recv_len;
    char buffer[256];

    // Pause the timer
    readTimer->stop();

    // Clear the buffer
    bzero(buffer, 256);

    // Receive a packet (blocking)
    recv_len = read(btSocket, buffer, 255);
    if (recv_len < 0) {
        fprintf(stderr, "Error reading data\n");
        return;
    }

    // Check if close packet received
    if (strcmp(buffer, "close") == 0) {
        close(btSocket);
        delete readTimer;
    } else {
        // Emit received data through signal
        QString str;
        str.sprintf("%s", buffer);
        emit dataFromThread(str);

        // Restart the timer
        readTimer->start(1);
    }*/
}
BluetoothDataThread::~BluetoothDataThread(){
    stop();
}

BluetoothDataThread::BluetoothDataThread(){
    btSocket = 0;
}

void BluetoothDataThread::stop(){
     qDebug() << "Delete socket";
    delete btSocket;
    btSocket = 0;

}

//! [connected]
void BluetoothDataThread::connected()
{
    qDebug()<<btSocket->peerName();
}

//! [readSocket]
void BluetoothDataThread::readSocket()
{
    qDebug() << "form socket";
    if (!btSocket)
      return;

    while (btSocket->canReadLine()) {
         qDebug() << "readline form socket";
        QByteArray line = btSocket->readLine();
        emit dataFromThread(QString::fromUtf8(line.constData(), line.length()));
    }
}
//! [readSocket]

/* openSocket
 * Opens a RFCOMM bluetooth socket and begins listening for data.
 */
void BluetoothDataThread::openSocket()
{
    btSocket = new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol);
    qDebug() << "Create socket";
    btSocket->connectToService(QBluetoothAddress(dest),0x0001,QIODevice::ReadWrite);
    qDebug() << "socket connected";

    connect(btSocket, SIGNAL(readyRead()), this, SLOT(readSocket()));
    connect(btSocket, SIGNAL(connected()), this, SLOT(connected()));
    //connect(btSocket, SIGNAL(disconnected()), this, SIGNAL(disconnected()));
    /*
    int s, status;
    struct sockaddr_rc addr={0};
    btSocket = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
    addr.rc_family = AF_BLUETOOTH;
    addr.rc_channel = 1;
    str2ba(dest, &addr.rc_bdaddr);
    status = connect(s,(struct sockaddr*)&addr, sizeof(addr));

    // Signal that the socket was opened successfully
    emit socketOpened(btSocket);

    // Start the data read timer
    readTimer = new QTimer(this);
    connect(readTimer, SIGNAL(timeout()), this, SLOT(listenForPacket()));
    readTimer->start(1);*/



}
