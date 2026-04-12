#include <cstdio>
#include "irrigation_controller.h"

// ===== UTILITY FUNCTIONS =====
float mapFloat(
    float x,
    float in_min,
    float in_max,
    float out_min,
    float out_max
) {
    if (in_max == in_min) return out_min;

    return (x - in_min) * (out_max - out_min) /
           (in_max - in_min) + out_min;
}

float clamp(float value, float minVal, float maxVal) {
    if (value < minVal) return minVal;
    if (value > maxVal) return maxVal;
    return value;
}

// ===== CONFIGURATION =====
// Declare and initialize PlantSensor struct, including pins for pumps and capacitive soil moisture
// sensors, soil moisture percentages, and moisture levels checkInterval is an hour in milliseconds.
/*
Pins for future plant sensors:
Pump pins: 3, 4, 5
Soil Moisture Sensor pins: A1, A2, A3
AirValue, use 550 until calibrated
WaterValue, use 275 until calibrated

Values to try for initialization, but tune based on soil type, pump flow rate, and pot size
wateringDuration:   3 seconds
settlingDelay:      15 seconds
minPumpOffTime:     8–10 seconds
wateringCooldown:   1 hour
*/
/*
PlantRuntime plantRuntime = {
    0.0,            // soilMoisturePercentage
    0,            // moistureLevel
    0,              // lastCheckTime
    0,              // pumpStartTime
    0,              // lastWaterTime
    false           // pumpRunning
};
*/

// ===== PLANT SENSOR CREATION =====
PlantRuntime createRuntime() {
  PlantRuntime runtime;

  runtime.soilMoisturePercentage = 0.0;
  runtime.moistureLevel = 0;
  runtime.lastCheckTime = 0;
  runtime.pumpStartTime = 0;
  runtime.lastWaterTime = 0;
  runtime.pumpRunning = false;
  runtime.lastPumpStopTime = 0;
  runtime.settling = false;

  // runtime.state = IDLE;

  return runtime;
}

PlantSensor createPlant(
  PlantHardware *hardware,
  SensorCalibration *calibration,
  PlantConfig *config,
  PlantRuntime *runtime
) {
  PlantSensor plant;

  plant.hardware = hardware;
  plant.calibration = calibration;
  plant.config = config;
  plant.runtime = runtime;

  return plant;
}

// ===== LOGIC FUNCTIONS =====
// Function to toggle pump on or off based on plant soil moisture
void updatePlant(PlantSensor &plantSensor, unsigned long currentTime){
  // Settling delay
  if (plantSensor.runtime -> settling) {
    if (currentTime - plantSensor.runtime -> lastPumpStopTime <
        plantSensor.config -> settlingDelay) {
      return;
    }
    plantSensor.runtime -> settling = false;
  }

  // Maintain pump timing if running
  if (plantSensor.runtime -> pumpRunning) {
    readMoisture(plantSensor);
    runPump(plantSensor, currentTime);
    return;
  }

  // Only check moisture periodically
  if (currentTime - plantSensor.runtime -> lastCheckTime < plantSensor.config -> checkInterval) {
    return;
  }
  plantSensor.runtime -> lastCheckTime = currentTime;

  readMoisture(plantSensor);

  // Start watering if dry AND cooldown expired
  if (plantSensor.runtime -> soilMoisturePercentage <= plantSensor.config -> lowMoisture && cooldownExpired(plantSensor, currentTime) && pumpOffLongEnough(plantSensor, currentTime)) {
    runPump(plantSensor, currentTime);
  }
}

void readMoisture(PlantSensor &plantSensor){
  plantSensor.runtime -> moistureLevel = plantSensor.hardware->readSensor(
                                            plantSensor.hardware->SoilMoistureSensorPin
                                          );
  plantSensor.runtime -> soilMoisturePercentage = mapFloat(plantSensor.runtime->moistureLevel,
                                                            plantSensor.calibration->AirValue,
                                                            plantSensor.calibration->WaterValue,
                                                            0,
                                                            100);
  plantSensor.runtime -> soilMoisturePercentage = clamp(plantSensor.runtime -> soilMoisturePercentage, 0.0f, 100.0f);
  char msg[96];
  snprintf(msg, sizeof(msg),
    "Raw moisture: %d. Moisture %%: %.2f",
    plantSensor.runtime->moistureLevel,
    plantSensor.runtime->soilMoisturePercentage
  );
  plantSensor.hardware->log(msg);
}

void runPump(PlantSensor &plantSensor, unsigned long currentTime){
  // Start watering if not already running
  if (!plantSensor.runtime -> pumpRunning) {
    plantSensor.hardware->writePump(
      plantSensor.hardware->PumpPin,
      HIGH
    );
    plantSensor.runtime -> pumpRunning = true;
    plantSensor.runtime -> pumpStartTime = currentTime;
    plantSensor.hardware->log("Pump ON");
    return;
  }

  // Stop if moisture high enough
  if (plantSensor.runtime -> soilMoisturePercentage >=
      plantSensor.config -> highMoisture) {

    plantSensor.hardware->writePump(
      plantSensor.hardware->PumpPin,
      LOW
    );
    plantSensor.runtime -> pumpRunning = false;
    plantSensor.runtime -> lastPumpStopTime = currentTime;
    plantSensor.runtime -> settling = true;
    plantSensor.runtime -> lastWaterTime = currentTime;

    plantSensor.hardware->log("Moisture threshold reached, pump OFF");
    return;
  }

  // Stop watering after burst
  if (currentTime - plantSensor.runtime -> pumpStartTime >= plantSensor.config -> wateringDuration) {
    
    plantSensor.hardware->writePump(
      plantSensor.hardware->PumpPin,
      LOW
    );
    plantSensor.runtime -> pumpRunning = false;
    plantSensor.runtime -> lastPumpStopTime = currentTime;
    plantSensor.runtime -> settling = true;
    plantSensor.runtime -> lastWaterTime = currentTime;

    plantSensor.hardware->log("Pump OFF");
  }
}

bool cooldownExpired(PlantSensor &plantSensor, unsigned long currentTime) {
  return currentTime - plantSensor.runtime -> lastWaterTime >= plantSensor.config -> wateringCooldown;
}

bool pumpOffLongEnough(PlantSensor &plant, unsigned long currentTime) {
  return currentTime - plant.runtime->lastPumpStopTime >=
         plant.config->minPumpOffTime;
}