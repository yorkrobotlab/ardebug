/*
 * debug_network.cpp
 *
 * This class encapsulates networking functionality for sending packets of
 * robot data to the ARDebug application.
 *
 * (C) Alistair Jewers, Feb 2017.
 */
#include "debug_network.h"

#include <sstream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <iostream>

DebugNetwork::DebugNetwork(void) { }
DebugNetwork::~DebugNetwork(void) { }

/* init
 * Initialise the debugging system and network socket.
 * Must provide a port number, and fallback server IP and robot ID,
 * in case ardebug_config.txt cannot be found.
 */
void DebugNetwork::init(int port, std::string default_server_ip, std::string default_robot_id) {
    // Open config file
    std::ifstream file ("ar_debug_config.txt");

    if (file.is_open()) {
        // Config file open, acquire ID
        getline(file, this->robot_id);

        // Acquire IP
        getline(file, default_server_ip);

        file.close();
    } else {
        // Config file failure, use supplied values
        this->robot_id = default_robot_id;
    }

    // Networking setup

    // Initialise socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
        return;
    }

    // Zero the structure
    bzero(&sock_in, sizeof(sock_in));

    // Set up the socket structure
    sock_in.sin_port = htons(port);
    sock_in.sin_family = AF_INET;

    // Convert the server ip string to binary
    if (inet_aton(default_server_ip.c_str(), &sock_in.sin_addr) <= 0) {
        return;
    }

    socket_ready = true;

    std::cout << "Connected to: " << default_server_ip << std::endl;
}

/* destroy
 * Shut down the network socket.
 */
void DebugNetwork::destroy(void) {
    // Close the socket
    close(sockfd);

    // Clear ready flag
    socket_ready = false;
}

/* sendData
 * Send an arbitrary string of data to the debugging application.
 */
void DebugNetwork::sendData(std::string data) {
    if (socket_ready) {
        // Send the data
        sendto(sockfd, data.c_str(), data.length(), 0, (struct sockaddr *)&sock_in, sizeof(sock_in));
    }
}

/* getRobotID
 * Get the current robot ID value used when sending packets.
 */
std::string DebugNetwork::getRobotID(void) {
    return robot_id;
}