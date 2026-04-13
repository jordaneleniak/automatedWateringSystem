#ifndef MOCK_HARDWARE_H
#define MOCK_HARDWARE_H

int mockReadSensor(int pin);
void mockWritePump(int pin, int value);
void mockLog(const char* message);

#define HIGH 1

#define LOW 0

#endif // MOCK_HARDWARE_H