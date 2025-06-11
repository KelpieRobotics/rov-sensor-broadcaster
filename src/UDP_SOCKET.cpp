#include "UDP_SOCKET.h"

UDP_SOCKET::UDP_SOCKET(const std::string& ip, uint16_t port, const char* dev) : ip(ip), port(port), dev(dev) {
    initializeSocket();
    initSHT30(dev);
}

void UDP_SOCKET::initializeSocket()
{
    sockfd = socket(AF_INET, SOCK_DGRAM, 0); 
    if(sockfd < 0)
    {
        std::cerr << "Failed to create a socket. \n";
        std::exit(1);
    }
    std::cout << "Socket created" << std::endl;

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    if(bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)) < 0)
    {
        std::cerr << "Failed to bind socket" << std::endl;
        std::exit(1);
    }
    std::cout << "Socket binded" << std::endl;
}

void UDP_SOCKET::sendPacket()
{
    // read data from sensor
    readSHT30();
    float temperature = getTemperatureSHT30();
    float humidity = getHumiditySHT30();

    // protobuf msgs
    kelpie_sht_msgs::SHTChannels msg;
    msg.set_temperature(temperature);
    msg.set_humidity(humidity);

    // serialize
    uint8_t buffer[1024];
    int size = msg.ByteSizeLong();
    if(!msg.SerializeToArray(buffer, size))
    {
        std::cerr << "Failed to serialize!\n";
        return;
    }

    sockaddr_in destination{};
    destination.sin_family = AF_INET;
    destination.sin_port = htons(port);
    destination.sin_addr.s_addr = INADDR_ANY;

    ssize_t sent = sendto(sockfd, buffer, size, 0, (struct sockaddr*)&destination, sizeof(destination));
    if(sent < 0)
    {
        std::cerr << "Failed to send packet\n";
    } else 
    {
        std::cout << "Packet sent successfully!\n";
    }


}
