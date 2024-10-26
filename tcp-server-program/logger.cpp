#include "sht30/sht30.h"

#include <stdio.h>
#include "ds18b20/ds18b20.h"

int main() {
    initSHT30();
    while(true) {
        readSHT30();
        printf("temp: %f, humd: %f, external Temperature: %f\n", getTemperatureSHT30(), getHumiditySHT30(),readDS18B20());
    }
}
