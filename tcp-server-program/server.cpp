#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>

#define MAX 1024
#define PORT 8080
#define MAXLINE 1024
#define SA struct sockaddr


void send_temp(float temperature, int sockfd) {
    char buffer[256];
    snprintf(buffer, sizeof(buffer), "Temperature: %.2f\n", temperature);

    // send the temperature data over tcp
    write(sockfd, buffer, sizeof(buffer));
    bzero(buffer, sizeof(buffer));
    
}

void func(int sockfd)
{
    float temp = 70.5;
    send_temp(temp, sockfd);
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







