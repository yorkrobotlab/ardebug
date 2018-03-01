/*
 * debug_network.h
 *
 * This class encapsulates networking functionality for sending packets of
 * robot data to the SwarmDebug application.
 *
 * (C) Alistair Jewers, Feb 2017.
 */
#ifndef DEBUG_NETWORK_H
#define DEBUG_NETWORK_H

#include <string>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

// Packet types
#define PACKET_TYPE_WATCHDOG            0
#define PACKET_TYPE_STATE               1
#define PACKET_TYPE_POSITION            2
#define PACKET_TYPE_PROXIMITY           3
#define PACKET_TYPE_BACKGROUND_IR       4
#define PACKET_TYPE_MSG                 5
#define PACKET_TYPE_CUSTOM              6

// Packet types as space seperated strings
#define PACKET_TYPE_STR_WATCHDOG        " 0 "
#define PACKET_TYPE_STR_STATE           " 1 "
#define PACKET_TYPE_STR_POSITION        " 2 "
#define PACKET_TYPE_STR_PROXIMITY       " 3"
#define PACKET_TYPE_STR_BACKGROUND_IR   " 4"
#define PACKET_TYPE_STR_MSG             " 5 "
#define PACKET_TYPE_STR_CUSTOM          " 6 "

class DebugNetwork 
{
public:
    DebugNetwork(void);
    ~DebugNetwork(void);

    void init(int port, std::string default_server_ip, int default_robot_id);
    void destroy(void);

    // Raw packet function
    void sendData(std::string data);

    // Specific packet functions
    void sendWatchdogPacket(std::string name);
    void sendStatePacket(std::string state);
    void sendIRDataPacket(int* data, int count, bool background);
    void sendLogMessage(std::string message);
    void sendCustomData(std::string key, std::string value);

    // Robot ID functions
    int getRobotID(void);
    void setRobotID(int id);

private:
    // Network variables
    bool socket_ready;
    struct sockaddr_in sock_in;
    int sockfd;

    // Robot id within the swarm debug system
    int robot_id;
};

#endif /* DEBUG_NETWORK_H */