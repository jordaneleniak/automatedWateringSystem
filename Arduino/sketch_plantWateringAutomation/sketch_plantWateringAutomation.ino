/**
 * @file sketch_plantWateringAutomation.ino
 * @brief Arduino-based automated plant watering system.
 * @details
 * This program monitors soil moisture levels and controls water pumps to
 * maintain optimal soil conditions for plants. The system performs periodic
 * moisture checks using non-blocking timing (millis()) and applies burst
 * watering with settling delays to improve accuracy and prevent overwatering.
 * The system operates using burst watering cycles with settling delays to
 * ensure accurate moisture readings and stable soil hydration.
 * @author Jordan Eleniak
 * @version 0.7.0
 * @date 2026-04-09
 *
 * Features:
 * - Soil moisture calibration (air/water reference values)
 * - Burst watering with configurable duration
 * - Settling delay between bursts to allow soil absorption
 * - Minimum pump-off time to prevent rapid cycling and hardware damage
 * - Cooldown period to prevent overwatering
 * - Scalable structure supporting multiple plants
 *
 * Hardware:
 * - Arduino (UNO currently, planned ESP32-S3 migration)
 * - Capacitive soil moisture sensors
 * - 4 Channel 5V Relay Module
 * - Water pumps controlled by relay
 *
 * @note
 * Calibration values must be adjusted per sensor for accurate readings.
 * Core logic functions will avoid direct use of Arduino APIs and instead receive inputs (e.g., time) as parameters to enable unit testing.
*/

#include <Arduino.h>
#include "../../lib/irrigation_controller/irrigation_controller.h"

/*

*/

// ===== ARDUINO IMPLEMENTATIONS =====
int arduinoReadSensor(int pin) {
  return analogRead(pin);
}

void arduinoWritePump(int pin, int value) {
  digitalWrite(pin, value);
}

void arduinoLog(const char* message) {
  Serial.println(message);
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
PlantHardware pinLayout1 = {
    2,                  // PumpPin
    A0,                 // SoilMoistureSensorPin
    arduinoReadSensor,  // ReadSensorFunc points to arduinoReadSensor
    arduinoWritePump,    // WritePumpFunc points to arduinoWritePump
    arduinoLog           // LogFunc points to arduinoLog
};
SensorCalibration calibration1 = {
  564,              // AirValue
  271               // WaterValue
};
PlantConfig plantConfig1 = {
    35,             // lowMoisture
    65,             // highMoisture
    5000,           // checkInterval
    1000,           // wateringDuration
    3600000,        // wateringCooldown, an hour in milliseconds
    10000,          // minPumpOffTime, 10 seconds
    15000,          // settlingDelay, 15 seconds
};
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
PlantRuntime runtime = createRuntime();

PlantSensor plants[] = {
  createPlant(&pinLayout1, &calibration1, &plantConfig1, &runtime)
};

const int arrayLength = sizeof(plants) / sizeof(plants[0]);

// ===== SETUP / LOOP =====
void setup() {
  // put your setup code here, to run once:
  
  // Set the data rate in bits per second (baud) for serial data transmission
  Serial.begin(9600);
  
  // Initialize pinmodes for water pumps and capacitive soil moisture sensors, and digital writes to set high/low values to digital pins
  for (int i = 0; i < arrayLength; i++) {
    pinMode(plants[i].hardware -> PumpPin, OUTPUT);
    pinMode(plants[i].hardware -> SoilMoistureSensorPin, INPUT);
    digitalWrite(plants[i].hardware -> PumpPin, LOW);
  }

  // Pause program for a half second before re-running
  delay(500);
}

void loop() {
  // put your main code here, to run repeatedly:

  unsigned long now = millis();

  // Toggle pump 1 on or off based on plant soil moisture
  for (int i = 0; i < arrayLength; i++) {
    updatePlant(plants[i], now);
  }
  //delay(1000);
}
