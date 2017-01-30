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

void DataThread::connectToServer() {
    int sockfd, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];

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

    QString str;

    // Read data from the socket into the buffer
    do {
        bzero(buffer, 256);
        n = read(sockfd, buffer, 255);
        if (n < 0) {
            fprintf(stderr, "Error reading from socket\n");
            return;
        }

        // Emit the data to the main thread for processing (Qt signals and slots)
        str.sprintf("%s", buffer);
        emit(dataFromThread(str));
    } while (!strcmp(buffer, "Exit\n"));

    // Close the socket before finishing
    close(sockfd);
}
