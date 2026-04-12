#ifndef IRRIGATION_CONTROLLER_H
#define IRRIGATION_CONTROLLER_H

// ===== TYPEDEFS =====
typedef int (*ReadSensorFunc)(int pin);
typedef void (*WritePumpFunc)(int pin, int value);
typedef void (*LogFunc)(const char* message);

// ===== STRUCTS =====
struct PlantHardware {
  const int PumpPin;
  const int SoilMoistureSensorPin;

  ReadSensorFunc readSensor;
  WritePumpFunc writePump;
  LogFunc log;
};

struct SensorCalibration {
  const int AirValue;
  const int WaterValue;
};

struct PlantConfig {
  const int lowMoisture;
  const int highMoisture;

  const unsigned long checkInterval;
  const unsigned long wateringDuration;
  const unsigned long wateringCooldown;

  const unsigned long minPumpOffTime;
  const unsigned long settlingDelay;
};

struct PlantRuntime {
  float soilMoisturePercentage;
  int moistureLevel;

  unsigned long lastCheckTime;
  unsigned long pumpStartTime;
  unsigned long lastWaterTime;

  bool pumpRunning;

  unsigned long lastPumpStopTime;
  bool settling;
};

struct PlantSensor {
  PlantHardware *hardware;
  SensorCalibration *calibration;
  PlantConfig *config;
  PlantRuntime *runtime;
};

// ===== UTILITY FUNCTIONS =====
float mapFloat(float x, float in_min, float in_max, float out_min, float out_max);
float clamp(float value, float minVal, float maxVal);

// ===== LOGIC FUNCTIONS =====
void updatePlant(PlantSensor &plantSensor, unsigned long currentTime);
void readMoisture(PlantSensor &plantSensor);
void runPump(PlantSensor &plantSensor, unsigned long currentTime);
bool cooldownExpired(PlantSensor &plantSensor, unsigned long currentTime);
bool pumpOffLongEnough(PlantSensor &plant, unsigned long currentTime);

#define HIGH 1

#define LOW 0

#endif