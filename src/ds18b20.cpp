#include "ros-temperature-broadcaster/ds18b20.h"

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>

float readDS18B20() {
    char dev[16]{};
    char path[1024]{};
    char temp[6]{};
    
    float temperature = 0.0f;

    int fd = open("/sys/bus/w1/devices/w1_bus_master1/w1_master_slaves", O_RDONLY);

    if(fd > 0) {
        ssize_t bytes_read = read(fd, dev, 16);
        dev[bytes_read-1] = 0; // replace \n with \0
        strcat(path, "/sys/bus/w1/devices/");
        strcat(path, dev);
        strcat(path, "/temperature");

        // printf("%s\n", path);

        close(fd);
        fd = open(path, O_RDONLY);

        if(fd >= 0) {
            bytes_read = read(fd, temp, 6);
            temp[bytes_read-1] = 0; // replace \n with \0
            char* endptr;
            long temp_int = strtol(temp, &endptr, 10);
            if (endptr == temp) {
                printf("No digits were found.\n");
            } else if (*endptr != '\0') {
                printf("Invalid character: %c\n", *endptr);
            }
            else temperature = (float)temp_int/1000;
        }
        else printf("Error occured while opening device\n");
    }
    else printf("Error occured while opening device directory\n");

    return temperature;
}
