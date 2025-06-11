#pragma once

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include "sht30/sht30.h"
#include <chrono>

//#include "selk_rc_msgs/build/cpp/rov_sensor_broadcaster.pb.h"


class UDP_SOCKET
{
public:
    UDP_SOCKET(const std::string& ip, uint16_t port, const char* dev);
    void sendPacket();
private:
    int sockfd;
    std::string ip;
    uint16_t port;
    const char* dev;

    void initializeSocket();
};
