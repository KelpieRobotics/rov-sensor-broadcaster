#define SHT30_ADDR 0x44
#define TEMP_SENSOR1 {0x28, 0x13, 0x97, 0x9A, 0x0A, 0x00, 0x00, 0x6A}

//#define MAX 80
//#define PORT 8080
//#define MAXLINE 1024
//#define SA struct sockaddr

void initSHT30();
bool setTimeoutSHT30();
void readSHT30();
float getTemperatureSHT30();
float getHumiditySHT30();
