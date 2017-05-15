/*
 * debug_network.cpp
 *
 * This class encapsulates networking functionality for sending packets of
 * robot data to the SwarmDebug application.
 *
 * (C) Alistair Jewers, Feb 2017.
 */
#include "debug_network.h"

#include <sstream>

DebugNetwork::DebugNetwork(void) { }
DebugNetwork::~DebugNetwork(void) { }

/* init
 * Initialise the debugging system and network socket.
 */
void DebugNetwork::init(int port, std::string server_ip, int robot_id) {
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
    if (inet_aton(server_ip.c_str(), &sock_in.sin_addr) <= 0) {
        return;
    }

    socket_ready = true;

    // Set robot id
    this->robot_id = robot_id;
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

/* sendWatchdogPacket
 * Send a watchdog data packet to the debugging application.
 */
void DebugNetwork::sendWatchdogPacket(std::string name) {
    std::ostringstream packet;

    packet << robot_id << PACKET_TYPE_STR_WATCHDOG << name;

    sendData(packet.str());
}

/* sendStatePacket
 * Send a state data packet to the debugging application.
 */
void DebugNetwork::sendStatePacket(std::string state) {
    std::ostringstream packet;

    packet << robot_id << PACKET_TYPE_STR_STATE << state;

    sendData(packet.str());
}

/* sendIRDataPacket
 * Send an IR data packet to the debugging application. Can be active or
 * background readings.
 */
void DebugNetwork::sendIRDataPacket(int* data, int count, bool background) {
    std::ostringstream packet;

    if (background) {
        packet << robot_id << PACKET_TYPE_STR_BACKGROUND_IR;
    } else {
        packet << robot_id << PACKET_TYPE_STR_PROXIMITY;
    }	

    for (int i = 0; i < count; i++) {
        packet << " " << data[i];
    }

    sendData(packet.str());
}

/* sendLogMessage
 * Send a log message packet to the debugging application.
 */
void DebugNetwork::sendLogMessage(std::string message) {
    std::ostringstream packet;

    packet << robot_id << PACKET_TYPE_STR_MSG << message;

    sendData(packet.str());
}

/* sendCustomData
 * Send a custom data packet to the debugging application.
 */
void DebugNetwork::sendCustomData(std::string key, std::string value) {
    std::ostringstream packet;

    packet << robot_id << PACKET_TYPE_STR_CUSTOM << key << " " << value;

    sendData(packet.str());
}

/* getRobotID
 * Get the current robot ID value used when sending packets.
 */
int DebugNetwork::getRobotID(void) {
    return robot_id;
}

/* setRobotID
 * Change the current robot ID value used when sending packets.
 */
void DebugNetwork::setRobotID(int id) {
    this->robot_id = id;
}
