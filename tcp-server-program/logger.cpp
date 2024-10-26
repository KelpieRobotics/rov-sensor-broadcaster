#include "sht30/sht30.h"

#include <stdio.h>

int main() {
    initSHT30();
    while(true) {
        readSHT30();
        printf("temp: %f, humd: %f\n", getTemperatureSHT30(), getHumiditySHT30());
    }
}
