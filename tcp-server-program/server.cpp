#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "sht30/sht30.h"
#include "ds18b20.h"

#define MAX_BUFFER_SIZE 1024
#define PORT 8080
#define MAXLINE 1024
#define SA struct sockaddr


void send_temp(float int_temperature, float int_humidity, float externalTemperature, int sockfd) {
    char buffer[MAX_BUFFER_SIZE];
    int len = snprintf(buffer, sizeof(buffer), "Internal Temperature: %.2f\nInternal Humidity: %.2f\nExternal Temperature: %.2f\n", int_temperature, int_humidity,externalTemperature);

    // send the temperature data over tcp
    printf("Sending: %s", buffer);
    write(sockfd, buffer, len * sizeof(char));
    bzero(buffer, sizeof(buffer));
} 

void func(int sockfd)
{
    readSHT30();
    send_temp(getTemperatureSHT30(), getHumiditySHT30(), readDS18B20(), sockfd);
}

int main() {
    // tcp server
    int sockfd, connfd; 
    unsigned int len;
    struct sockaddr_in servaddr, cli; 

    // socket create and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        printf("socket creation failed...\n");
        exit(1);
    }

    bzero(&servaddr, sizeof(servaddr));

    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    // binding newly created socket to given IP and verification
    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
        printf("socket bind failed...\n");
        exit(1);
    }

    initSHT30();

    // now server is ready to listen and verification
        if ((listen(sockfd, 5)) != 0) {
            printf("Listen failed...\n");
            exit(1);
        }

    for(;;){

        // accept the data packet from client and verification
        connfd = accept(sockfd, (SA*)&cli, &len);
        if (connfd < 0) {
            printf("server accept failed...\n");
            exit(1);
        }

        func(connfd);
    }
   

    return 0;
}
