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
 * - Set up testing infrastructure (test/ directory, AUnit for Arduino, native unit testing for logic)
 * - Refactor logic into unit-testable functions (remove direct millis(), digitalWrite(), etc. from core logic)
 * - Perform initial tuning of thresholds and timing based on test results
 * - Implement structured test plan using Analog Discovery 2 to validate control logic (simulate moisture signals, verify pump behavior, timing, and thresholds)
 * - Add documentation code blocks and improve comments for code readability
 * - Convert control logic to state machine:
 *   - Lifecycle states: UNINITIALIZED, INITIALIZING, READY
 *   - Operational states: IDLE, WATERING, SETTLING, COOLDOWN
 *   - Events/actions:
 *       - RESET (forces transition to UNINITIALIZED)
 * - Implement plant initialization, reset handling, and persistent state (EEPROM/RTC integration)
 * - Perform real-world tuning based on soil testing
 * - Refactor project into modular header/source files (lib/ structure)
 * - Complete STM32 bring-up and port core watering logic (ADC, GPIO, timing)
 * - Add reliability features (pump safety limits, reservoir monitoring, sensor error handling)
 * - Add RTC/NTP and seasonal watering logic
 * - Implement ESP32 version with WiFi monitoring and UI
*/

 #include <Arduino.h>

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
1. Run structured test plan (Analog Discovery 2)
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
[ ] Create structured test plan using Analog Discovery 2
[ ] Simulate moisture signals (triangle, step, edge cases)
[ ] Verify:
    - pump start/stop behavior
    - burst duration timing
    - settling delay behavior
    - minimum pump-off enforcement
    - cooldown logic
[ ] Add serial logging for state transitions and debugging
[ ] Set up test directory structure (test/arduino, test/native)
[ ] Add initial unit tests for core logic (cooldown, thresholds, timing)
[ ] Separate hardware-dependent code from logic for testing

Code Organization
-----------------
[ ] Refactor logic into testable units
    - Remove direct use of millis(), digitalWrite(), etc.
    - Pass hardware inputs as parameters

Control Logic
-------------
[ ] Convert watering logic to a state machine
    - Implement lifecycle states (UNINITIALIZED, INITIALIZING, READY)
    - Implement operational states (IDLE, WATERING, SETTLING, COOLDOWN)
    - Define events/actions:
        - RESET (resets plant to UNINITIALIZED state)
    - Ensure initialization is required before entering normal operation
[ ] Add helper functions for readability (cooldownExpired, settlingComplete, etc.)
[ ] Validate logic under edge cases (rapid signal changes, threshold boundaries)

State Initialization & Persistence
---------------------------------
[ ] Implement plant initialization phase (baseline moisture reading)
[ ] Add per-plant reset capability (sensor reassignment)
[ ] Design persistent state structure (EEPROM or future RTC-based)
[ ] Handle safe startup after power loss (startup delay + sensor check)
[ ] Ensure system does not water immediately after boot

STM32 Migration
---------------
[ ] Generate STM32 project (L432KC)
[ ] Configure GPIO for pump control
[ ] Configure ADC for moisture sensor input
[ ] Implement timing using HAL_GetTick()
[ ] Port core watering logic from Arduino
[ ] Validate behavior matches Arduino implementation

Hardware & Reliability
----------------------
[ ] Add pump runtime safety limits (max runtime per hour)
[ ] Add reservoir level sensor support
[ ] Add error handling for sensor failures
[ ] Validate relay and pump behavior under load

Calibration & Tuning
--------------------
[ ] Define calibration procedure (airValue, waterValue)
[ ] Validate mapping from raw ADC to percentage
[ ] Tune burst duration, settling delay, and cooldown using real soil
[ ] Adjust thresholds for different soil types

Time & Seasonal Logic
---------------------
[ ] Integrate RTC or NTP time source
[ ] Implement seasonal watering profiles per plant
[ ] Adjust watering behavior based on time/temperature

ESP32 Migration
---------------
[ ] Port sketch from Arduino to ESP32-S3
[ ] Verify ADC calibration and resolution differences
[ ] Validate relay/pump compatibility with 3.3V logic
[ ] Integrate WiFi connectivity

Monitoring & UI
---------------
[ ] Add OLED or LCD status display
[ ] Implement WiFi monitoring dashboard
[ ] Display plant moisture, system state, and last watering time

Code Organization
-----------------
[ ] Split structs and logic into header/source files
[ ] Separate hardware abstraction from control logic
[ ] Create reusable core logic layer for Arduino/STM32/ESP32
[ ] Add logging/debug framework for testing and validation

----------------------
Last Verified Working:
----------------------
[ ] Basic functionality implemented but not formally tested

----------------------
Verification Checklist (to be completed during testing phase):
----------------------
[ ] millis()-based scheduling (timing accuracy and consistency)
[ ] burst watering logic (correct start/stop behavior)
[ ] settling delay (no immediate re-trigger after watering)
[ ] pump-off protection (prevents rapid cycling)
[ ] cooldown logic (enforces delay between watering cycles)
*/

// TODO: Implement ENUMS so it's used
// ===== ENUMS =====
enum PlantState {
  IDLE,
  WATERING,
  COOLDOWN
};

// ===== STRUCTS =====
struct PlantHardware {
  // Declare constant variables for pump pin and capacitive soil moisture sensor pins
  const int PumpPin;
  const int SoilMoistureSensorPin;
};

struct SensorCalibration {
  // Declare constant variables for air and water values. This is for calibrating the capacitive soil sensor
  const int AirValue;
  const int WaterValue;
};

struct PlantConfig {
  // Declare constant variables to set plants low and high soil moisture levels
  const int lowMoisture;
  const int highMoisture;

  // Declare variables to for checking watering intervals
  const unsigned long checkInterval;
  // Declare variables to determine how long to water
  const unsigned long wateringDuration; // wateringDuration is a a burst length
  // Declare variables to determine when the plant was last watered and how long to wait before the next watering
  const unsigned long wateringCooldown;

  // Declare variables for minimum pump off-time and a water settling delay
  const unsigned long minPumpOffTime;
  const unsigned long settlingDelay;
};

struct PlantRuntime {
  // Declare variables for soil moisture percentage and moisture level
  float soilMoisturePercentage;
  int moistureLevel;

  // Declare variables for when the soil moisture was last checked
  unsigned long lastCheckTime;
  // Declare variables for when the pump starts watering
  unsigned long pumpStartTime;
  // Declare variables to determine when the plant was last watered and how long to wait before the next watering
  unsigned long lastWaterTime;

  // Declare a variable to check if pump is running
  bool pumpRunning;

  // Declare variables for last pump stop time and settling
  unsigned long lastPumpStopTime;
  bool settling;
};

struct PlantSensor {
  PlantHardware *hardware;
  SensorCalibration *calibration;
  PlantConfig *config;
  PlantRuntime *runtime;
};

// ===== CONFIGURATION =====
// Declare and initialize PlantSensor struct, including pins for pumps and capacitive soil moisture sensors, soil moisture percentages, and moisture levels 
// checkInterval is an hour in milliseconds.
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
TODO: Verify that the checkInterval is an appropriate time.
      Remove delayTime when millis() is finished being implemented
      Determine watering duration time, will use 1 second set to milliseconds.
      Double check low and high moisture levels for plant
*/
PlantHardware pinLayout1 = {
    2,              // PumpPin
    A0              // SoilMoistureSensorPin
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

PlantRuntime runtime = createRuntime();

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

PlantSensor plants[] = {
  createPlant(&pinLayout1, &calibration1, &plantConfig1, &runtime)
};

const int arrayLength = sizeof(plants) / sizeof(plants[0]);

// ===== LOGIC FUNCTIONS =====
// Function to toggle pump on or off based on plant soil moisture
void updatePlant(PlantSensor &plantSensor){
  // Settling delay
  if (plantSensor.runtime -> settling) {
    if (millis() - plantSensor.runtime -> lastPumpStopTime <
        plantSensor.config -> settlingDelay) {
      return;
    }
    plantSensor.runtime -> settling = false;
  }

  // Maintain pump timing if running
  if (plantSensor.runtime -> pumpRunning) {
    readMoisture(plantSensor);
    runPump(plantSensor);
    return;
  }

  // Only check moisture periodically
  if (millis() - plantSensor.runtime -> lastCheckTime < plantSensor.config -> checkInterval) {
    return;
  }
  plantSensor.runtime -> lastCheckTime = millis();

  readMoisture(plantSensor);

  // Start watering if dry AND cooldown expired
  if (plantSensor.runtime -> soilMoisturePercentage <= plantSensor.config -> lowMoisture && cooldownExpired(plantSensor) && pumpOffLongEnough(plantSensor)) {
    runPump(plantSensor);
  }
}

void readMoisture(PlantSensor &plantSensor){
  plantSensor.runtime -> moistureLevel = analogRead(plantSensor.hardware -> SoilMoistureSensorPin);
  plantSensor.runtime -> soilMoisturePercentage = (float)map(plantSensor.runtime -> moistureLevel, 
                                                  plantSensor.calibration->AirValue, 
                                                  plantSensor.calibration->WaterValue, 
                                                  0, 
                                                  100);
  plantSensor.runtime -> soilMoisturePercentage = constrain(plantSensor.runtime -> soilMoisturePercentage, 0, 100);
  Serial.print("Raw moisture: ");
  Serial.print(plantSensor.runtime -> moistureLevel);
  Serial.print(". Moisture %: ");
  Serial.println(plantSensor.runtime -> soilMoisturePercentage);
}

void runPump(PlantSensor &plantSensor){
  // Start watering if not already running
  if (!plantSensor.runtime -> pumpRunning) {
    digitalWrite(plantSensor.hardware -> PumpPin, HIGH);
    plantSensor.runtime -> pumpRunning = true;
    plantSensor.runtime -> pumpStartTime = millis();
    Serial.println("Pump ON");
    return;
  }

  // Stop if moisture high enough
  if (plantSensor.runtime -> soilMoisturePercentage >=
      plantSensor.config -> highMoisture) {

    digitalWrite(plantSensor.hardware->PumpPin, LOW);
    plantSensor.runtime -> pumpRunning = false;
    plantSensor.runtime -> lastPumpStopTime = millis();
    plantSensor.runtime -> settling = true;
    plantSensor.runtime -> lastWaterTime = millis();

    Serial.println("Moisture threshold reached, pump OFF");
    return;
  }

  // Stop watering after burst
  if (millis() - plantSensor.runtime -> pumpStartTime >= plantSensor.config -> wateringDuration) {
    
    digitalWrite(plantSensor.hardware -> PumpPin, LOW);
    plantSensor.runtime -> pumpRunning = false;
    plantSensor.runtime -> lastPumpStopTime = millis();
    plantSensor.runtime -> settling = true;
    plantSensor.runtime -> lastWaterTime = millis();

    Serial.println("Pump OFF");
  }
}

bool cooldownExpired(PlantSensor &plantSensor) {
  return millis() - plantSensor.runtime -> lastWaterTime >= plantSensor.config -> wateringCooldown;
}

bool pumpOffLongEnough(PlantSensor &plant) {
  return millis() - plant.runtime->lastPumpStopTime >=
         plant.config->minPumpOffTime;
}

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
  // Toggle pump 1 on or off based on plant soil moisture
  for (int i = 0; i < arrayLength; i++) {
    updatePlant(plants[i]);
  }
  //delay(1000);
}
