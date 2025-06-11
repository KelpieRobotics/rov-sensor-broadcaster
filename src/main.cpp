#include "UDP_SOCKET.h"


int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        printf("Please specify a I2C path");
        return 1;
    }
    UDP_SOCKET sht_udp("127.0.0.1", 5005, argv[1]);
    sht_udp.sendPacket();

    return 0;
}