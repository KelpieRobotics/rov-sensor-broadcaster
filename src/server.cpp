#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "sht30/sht30.h"
//#include "ds18b20/ds18b20.h"

#define MAX_BUFFER_SIZE 1024
#define PORT 8080
#define MAXLINE 1024
#define SA struct sockaddr
#define SA_in struct sockaddr_in


void send_temp(float int_temperature, float int_humidity, int sockfd, SA_in* cli, socklen_t len) {
    char buffer[MAX_BUFFER_SIZE];
    int length = snprintf(buffer, sizeof(buffer), "Internal Temperature: %.2f\nInternal Humidity: %.2f\nExternal Temperature: %.2f\n", int_temperature, int_humidity);

    // send the temperature data over tcp
    printf("Sending: %s", buffer);
    sendto(sockfd, buffer, length, 0, (const SA*) cli, len);
} 

void func(int sockfd, struct sockaddr_in* cli, socklen_t len)
{
    readSHT30();
    send_temp(getTemperatureSHT30(), getHumiditySHT30(), sockfd, cli, len);
}

int main(int argc, char* argv[]) {
    if(argc < 2)
    {
        printf("Please specify the I2C path");
        return 1;
    }

    // select i2c path
    const char* i2c_path = argv[1];
    // udp server
    int sockfd;
    struct sockaddr_in servaddr, cli; 
    socklen_t len;
    char buffer[MAX_BUFFER_SIZE];

    // socket create and verification
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        printf("socket creation failed...\n");
        exit(1);
    }

    bzero(&servaddr, sizeof(servaddr));
    bzero(&cli, sizeof(cli));

    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    // binding newly created socket to given IP and verification
    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
        printf("socket bind failed...\n");
        exit(1);
    }

    initSHT30(i2c_path);

    // now server is ready to listen and verification
        if ((listen(sockfd, 5)) != 0) {
            printf("Listen failed...\n");
            exit(1);
        }

    for(;;){

        // accept the data packet from client and verification
        len = sizeof(cli);
        int n = recvfrom(sockfd, buffer, MAX_BUFFER_SIZE, 0, (SA*)&cli, &len);
        buffer[n] = '\0';

        printf("Received data: %s", buffer);
        func(sockfd, &cli, len);

    }
   

    return 0;
}
