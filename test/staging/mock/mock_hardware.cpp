#include "mock_hardware.h"

int mockSensorValue = 500;
bool pumpState = false;

int mockReadSensor(int pin) {
    return mockSensorValue;
}

void mockWritePump(int pin, int value) {
    pumpState = (value == HIGH);
}

void mockLog(const char* message) {
    // optional: store logs
}