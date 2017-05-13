/* util.cpp
 *
 * Utility functions used throughout the application.
 *
 * (C) Alistair Jewers Feb 2017
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "util.h"
#include "settings.h"

/* sendClosePacket
 * Sends a specific packet to indicate that the application should close
 * its socket.
 */
void sendClosePacket(int port) {
    struct sockaddr_in sock_in;
    int sockfd;
    char buf[6] = {'c', 'l', 'o', 's', 'e', '\0'};

    // Create the socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
        fprintf(stderr, "Error creating socket\n");
        return;
    }

    // Zero the structure
    bzero(&sock_in, sizeof(sock_in));

    // Set up the socket structure
    sock_in.sin_port = htons(port);
    sock_in.sin_family = AF_INET;

    // Convert ip address from string form to binary
    if (inet_aton("127.0.0.1", &sock_in.sin_addr) <= 0) {
        fprintf(stderr, "Error inet_aton\n");
        return;
    }

    // Send the buffer data
    sendto(sockfd, buf, 6, 0, (struct sockaddr *)&sock_in, sizeof(sock_in));

    // Close the socket
    close(sockfd);
}

/* square
 * Squares a value
 */
double square(double val) {
    return val * val;
}
