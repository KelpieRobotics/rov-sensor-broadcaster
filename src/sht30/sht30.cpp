#include "sht30.h"

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdint.h>

#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <sys/ioctl.h>
#include <errno.h>

int fd;
float temperature, humidity;

bool setTimeoutSHT30(int microseconds) {
    return ioctl(fd, I2C_TIMEOUT, microseconds/10) == 0;
}

void initSHT30() {
    fd = open("/dev/i2c-1", O_RDWR);

    if (fd < 0) {
        perror("Unable to open i2c file descriptor\n");
        
        // exit(1);
    }

    setTimeoutSHT30(400);
}

void readSHT30() {
    i2c_msg msg;
    i2c_rdwr_ioctl_data msgset;

    msg.addr = SHT30_ADDR;
    msg.flags = I2C_M_STOP;
    msg.len = 2;
    msg.buf = new uint8_t[2]{0x24, 0x00};

    msgset.msgs = &msg;
    msgset.nmsgs = 1;

    if (ioctl(fd, I2C_RDWR, &msgset) < 0)
    {
        switch(errno) {
            default:
                perror("Unknown error while attempting i2c write operation\n");
        }
    }

    sleep(1); // wait for measurment

    msg.flags = I2C_M_RD | I2C_M_STOP;
    msg.len = 6;
    msg.buf = new uint8_t[6];

    if (ioctl(fd, I2C_RDWR, &msgset) >= 0) {

        uint16_t temp_raw = (((uint16_t)msg.buf[0] << 8) & 0xFF00) | (uint16_t)msg.buf[1] & 0x00FF;  // get MSB + LSB temp bytes
        uint16_t humd_raw = (((uint16_t)msg.buf[3] << 8) & 0xFF00) | (uint16_t)msg.buf[4] & 0x00FF;  // get MSB + LSB temp bytes

        // convert temp and humdity
        temperature = -45 + 175 * (float)temp_raw / 65535.0;
        humidity = 100.0 * (float)humd_raw  / 65535.0;
    } else {
        printf("Failed to read!\n");
    }
}

float getTemperatureSHT30() {
    return temperature;
}

float getHumiditySHT30() {
    return humidity;
}
