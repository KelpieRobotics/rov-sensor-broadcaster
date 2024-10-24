// #include "TwoWire.h"

#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <tcl8.6/tcl-private/compat/unistd.h>

#define SHT30_ADDR 0x44
#define TEMP_SENSOR1 {0x28, 0x13, 0x97, 0x9A, 0x0A, 0x00, 0x00, 0x6A}

#define MAX 80
#define PORT 8080
#define MAXLINE 1024
#define SA struct sockaddr

// float readSHT30() {
//     TwoWire.beginTransmission(SHT30_ADDR);
//     TwoWire.write(0x2C);         // send SHT30 measurement command
//     TwoWire.write(0x06);         // high repeatability, no clock stretching
//     TwoWire.endTransmission();

//     delay(300); // wait for measurment

//     TwoWire.requestFrom(SHT30_ADDR, 6); // 6 bytes = 2 temp + 1 checksum + 2 hum + 1 checksum

//     if (TwoWire.available() == 6) {

//         uint16_t temp_raw = (TwoWire.read() << 8) | TwoWire.read();  // get MSB + LSB temp bytes
//         TwoWire.read();  // skip checksum

//         uint16_t hum_raw = (TwoWire.read() << 8) | TwoWire.read();   // get MSB + LSB humid bytes
//         TwoWire.read();  // skip checksum

//         // convert temp and humdity
//         float temperature = -45 + 175 * (float)temp_raw / 65535.0;
//         //float humidity = 100.0 * (float)hum_raw  / 65535.0;

//         return temperature;
//     } else {
//         printf("Failed to read!\n");
//     }
// }

void send_temp(float temperature, int sockfd) {
    char buffer[256];
    snprintf(buffer, sizeof(buffer), "Temperature: %.2f\n", temperature);

    // send the temperature data over tcp
    write(sockfd, buffer, sizeof(buffer));
    bzero(buffer, sizeof(buffer));

    close(sockfd);
}

void main() {
    // tcp server
    int sockfd, connfd, len; 
    struct sockaddr_in servaddr, cli; 

    // socket create and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("socket creation failed...\n");
        exit(0);
    }

    bzero(&servaddr, sizeof(servaddr));

    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    // binding newly created socket to given IP and verification
    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
        printf("socket bind failed...\n");
        exit(0);
    }

    // now server is ready to listen and verification
    if ((listen(sockfd, 5)) != 0) {
        printf("Listen failed...\n");
        exit(0);
    }

    // accept the data packet from client and verification
    connfd = accept(sockfd, (SA*)&cli, &len);
    if (connfd < 0) {
        printf("server accept failed...\n");
        exit(0);
    }

    func(connfd);

    //close(sockfd);
}

void func(int sockfd)
{
    for (;;) {
        // client set up
        // read temp from sensor
        // float temp = readSHT30();

        //test 
        float temp = 70.5;
        // send data to topside client
        send_temp(temp, sockfd);
    }

}





