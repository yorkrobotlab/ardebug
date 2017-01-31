#include "datathread.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include "QString"

void DataThread::connectToServer(const QString param) {
    struct sockaddr_in serv_addr;
    struct hostent *server;

    // Create a socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) {
        fprintf(stderr, "Error opening socket\n");
        return;
    }

    // Get the host entity
    server = gethostbyname("127.0.0.1");

    if (server == NULL) {
        fprintf(stderr, "Error, no such host\n");
        return;
    }

    // Create the server address structure
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(8001);

    // Attempt to connect the server to the socket
    if(::connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        fprintf(stderr, "Error connecting\n");
        return;
    }

    // Start the data read timer
    readTimer = new QTimer(this);
    connect(readTimer, SIGNAL(timeout()), this, SLOT(readDataFromServer()));
    readTimer->start(100);
}

void DataThread::readDataFromServer() {
    QString str;
    char buffer[256];
    int n;

    readTimer->stop();

    // Read data from the socket into the buffer
    bzero(buffer, 256);
    n = read(sockfd, buffer, 255);
    if (n < 0) {
        fprintf(stderr, "Error reading from socket\n");
        return;
    }

    // If an exit message is received, end the connection
    if (strcmp(buffer, "Exit\n") == 0) {
        // Close the socket, connection has ended
        close(sockfd);
        return;
    } else {
        // Otherwise pass the data to the main thread for processing
        str.sprintf("%s", buffer);
        emit(dataFromThread(str));
    }

    // Send a response
    if (disconnect) {
        n = write(sockfd, "Exit\n", 6);
    } else {
        n = write(sockfd, "resp\n", 6);
        readTimer->start(0.1);
    }

    if (n < 0) {
        fprintf(stderr, "Error reading from socket\n");
        return;
    }
}

void DataThread::disconnectFromServer(void) {
    disconnect = 1;
}
