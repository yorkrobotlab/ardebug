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

/* senClosePacket
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

/* colourGen
 * Generates a cv colour, different to the recently generated colours.
 */
cv::Scalar colourGen(void) {
    static const int r[] = { 0,   0,   0,   0,   0,   64,  128, 192, 255, 192, 128, 64  };
    static const int g[] = { 0,   64,  128, 192, 255, 192, 128, 64,  0,   0,   0,   0   };
    static const int b[] = { 255, 192, 128, 64,  0,   0,   0,   0,   0,   64,  128, 192 };
    static int i = 6;

    i++;
    if (i > 11) {
        i = -1;
    }

    if (Settings::instance()->isRobotColourEnabled()) {
        return cv::Scalar(b[i], g[i], r[i]);
    }

    return cv::Scalar(255, 255, 255);
}

double square(double val) {
    return val * val;
}
