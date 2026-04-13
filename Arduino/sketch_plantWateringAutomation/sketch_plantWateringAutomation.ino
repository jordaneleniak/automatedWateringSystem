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
 * @version 0.5
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
 *
 * @todo
 * Current Focus:
 * - Testing & Validation
 *   - Expand unit test coverage (Catch2)
 *   - Validate timing logic and edge cases
 *
 * Next Steps:
 * - Control Logic Improvements (state machine implementation)
 * - Platform Development (STM32 bring-up and logic port)
 *
 * Upcoming:
 * - Hardware & Reliability features
 * - Calibration & tuning with real soil data
 * - Monitoring & UI (display, WiFi dashboard)
 *
 * See full TODO list in project TODO block and README for details.
*/

#include <Arduino.h>
#include "../../lib/irrigation_controller/irrigation_controller.h"

// Get the voltage reading from capacitive soil moisture sensor
// Note: This is not used in control logic, but may be useful for testing,
// debugging, and calibration (e.g., when using a function generator).
//
// The sensor outputs approximately 0–3.3V. The ADC reads this as a raw value
// (rawMoistureLevel), which depends on the microcontroller’s ADC resolution
// and reference voltage.
//
// In this system, calibration values are defined:
// - airValue   ≈ 675 → dry soil / air (higher ADC value)
// - waterValue ≈ lower value → wet soil
//
// The control logic uses calibrated percentage (0–100%), NOT voltage.
// However, voltage can be estimated in two ways:
//
// 1) General ADC-based conversion (portable across platforms):
//    float voltage = rawMoistureLevel * (Vref / ADC_MAX);
//
//    where:
//    - Vref = ADC reference voltage (e.g., 5.0V for Arduino UNO, 3.3V for STM32/ESP32)
//    - ADC_MAX = maximum ADC value (UNO: 1023, STM32/ESP32: 4095)
//
// 2) Calibration-based approximation (based on dry value):
//    float voltage = rawMoistureLevel * (Vref / airValue);
//
//    Example (using calibrated dry value ≈ 675):
//    float voltage = rawMoistureLevel * (3.3 / 675);
//
//    This assumes airValue corresponds to ~3.3V and is less portable,
//    but can be useful for debugging relative sensor behavior.
//
// Summary:
// - Use percentage for control logic
// - Use voltage only for testing, debugging, and calibration insight

/*
===========================
NEXT SESSION PRIORITY
===========================
1. Run structured test plan
2. Verify all checklist items
3. Update "Last Verified Working"
===========================
*/

/*
===========================
Automated Watering System
Project TODO List
===========================

Testing & Validation
--------------------
[ ] Expand unit test coverage (Catch2)
[ ] Test plant creation and initialization logic
[ ] Test moisture reading and percentage conversion
[ ] Test pump control logic (start/stop conditions)
[ ] Test timing logic (cooldown, settling, min off time)
[ ] Validate edge cases:
    - rapid moisture fluctuations
    - threshold boundaries (low/high)
    - timing overflow-safe comparisons
[ ] Create structured test plan (Analog Discovery 2)
[ ] Simulate moisture signals (step, gradual, edge cases)
[ ] Verify:
    - pump start/stop behavior
    - burst duration timing
    - settling delay behavior
    - minimum pump-off enforcement
    - cooldown logic
[ ] Add serial logging for debugging and validation

Control Logic Improvements
--------------------------
[ ] Add documentation and comments for improved readability
[ ] Convert control logic to a state machine:
    - Lifecycle states: UNINITIALIZED, INITIALIZING, READY
    - Operational states: IDLE, WATERING, SETTLING, COOLDOWN
    - RESET event/action
[ ] Add helper functions (cooldownExpired, settlingComplete, etc.)
[ ] Improve readability and maintainability of control flow

Platform Development
--------------------
[ ] Complete STM32 bring-up (L432KC)
    - Configure GPIO for pump control
    - Configure ADC for moisture sensor input
    - Implement timing using HAL_GetTick()
    - Port core watering logic
    - Validate behavior matches Arduino
[ ] Implement ESP32 version
    - Verify ADC calibration and resolution
    - Validate relay/pump compatibility (3.3V logic)
    - Add WiFi capability and monitoring

Code Organization
-----------------
[ ] Finalize separation of logic and hardware layers
[ ] Expand reusable core logic module (irrigation_controller)
[ ] Refactor into header/source files (where needed)
[ ] Add logging/debug framework

Hardware & Reliability
----------------------
[ ] Add pump runtime safety limits (max runtime per hour)
[ ] Add reservoir level sensor support
[ ] Add sensor failure detection and handling
[ ] Validate relay and pump behavior under load

Calibration & Tuning
--------------------
[ ] Define calibration procedure (AirValue, WaterValue)
[ ] Validate ADC → percentage mapping
[ ] Tune:
    - watering duration
    - settling delay
    - cooldown timing

Time & Smart Features
---------------------
[ ] Integrate RTC or NTP time source
[ ] Implement seasonal watering profiles
[ ] Adjust watering behavior based on time/environment

Monitoring & UI
---------------
[ ] Add OLED/LCD display
[ ] Display:
    - moisture level
    - system state
    - last watering time
[ ] Implement WiFi dashboard (ESP32)

State Management (Planned)
--------------------------
[ ] Implement initialization phase (baseline moisture detection)
[ ] Add per-plant reset capability
[ ] Add persistent state storage (EEPROM / RTC / NTP)
[ ] Implement safe startup handling after power loss

----------------------
Last Verified Working:
----------------------
[ ] Basic functionality implemented but not formally tested

----------------------
Verification Checklist:
----------------------
[ ] millis()-based scheduling
[ ] burst watering logic
[ ] settling delay
[ ] pump-off protection
[ ] cooldown logic
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
