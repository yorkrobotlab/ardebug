/* datathread.cpp
 *
 * This class encapsulates a threaded UDP packet receiver. Data
 * that is received is sent to the main thread via the Qt signals
 * and slots system.
 *
 * (C) Alistair Jewers Jan 2017
 */

#include "datathread.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include <iostream>

#include "logging.h"

#include "QString"

DataThread::DataThread(QObject *parent)
    : QThread(parent)
{

}

/* Destructor
 * Delete necessary data.
 */
DataThread::~DataThread(void) { }

/* openUDPSocket
 * Opens a UDP socket on the given port, and begins listening for data.
 */
void DataThread::openUDPSocket(int port) {
    socket = new QUdpSocket;

    // Bind the socket
    if (!socket->bind(port)) {
        std::cerr<<"Error binding socket\n"<<std::endl;
        return;
    }

    emit socketOpened(port);
}

/* listenForPacket
 * Listens for data on the UDP socket. Blocking.
 */
void DataThread::run(void) {
    while(!socket && shouldRun) usleep(10);
    if(!socket) return; // We have broken out of the above loop without a valid socket so we should end here

    while(socket->state() != QAbstractSocket::BoundState && shouldRun) usleep(10);

    std::cout<<"Listening on data thread"<<std::endl;

    while(shouldRun && socket->state() == QAbstractSocket::BoundState)
    {
        while(!socket->hasPendingDatagrams() && shouldRun) usleep(1);
        if(!shouldRun) break; // We should stop running here

        int maxSize = socket->pendingDatagramSize()+20;
        char buffer[maxSize];

        // Receive a packet (blocking)
        int size;
        if((size = socket->readDatagram(buffer, maxSize)) > 0)
        {
            QString str = QString::fromLocal8Bit(buffer, size);
            emit dataFromThread(str);
        }
   }

    socket->deleteLater();
}
