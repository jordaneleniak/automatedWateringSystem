// float voltage1 = moistureLevel1 * (3.3 / 675); // Get the voltage reading from capacitive soil moisture sensor

/*
===========================
Automated Watering System
Project TODO List
===========================

Control Logic
-------------
[ ] Convert watering logic to a state machine (IDLE, WATERING, SETTLING, COOLDOWN)
[ ] Add helper functions for readability (cooldownExpired, settlingComplete, etc.)
[ ] Tune burst duration and settling delay based on real soil testing
[ ] Deep sleep modes (ESP32)
[ ] Add Interrupt-driven low-water detection to prevent pump dry-run

Hardware & Reliability
----------------------
[ ] Add pump runtime safety limits (max runtime per hour)
[ ] Add reservoir level sensor support
[ ] Add error handling for sensor failures 

ESP32 Migration
---------------
[ ] Port sketch from Arduino UNO to ESP32-S3
[ ] Verify ADC calibration and resolution on ESP32
[ ] Validate relay/pump compatibility with 3.3V logic

Time & Seasonal Logic
---------------------
[ ] Integrate RTC or NTP time source
[ ] Implement seasonal watering profiles per plant
[ ] Adjust watering thresholds based on temperature or season
[ ] Adjust watering times based on time of day (morning/evening watering cycles) and potentially based on sun-rise/set tables

Monitoring & UI
---------------
[ ] Add OLED or LCD status display
[ ] Implement WiFi monitoring dashboard
[ ] Show plant moisture, state, and last watering time

Code Organization
-----------------
[ ] Split structs and logic into header/source files
[ ] Separate plant logic from hardware abstraction layer
[ ] Add logging framework for debugging and tuning

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
  // Declare constant variables for pump pin and capacitive sould moisture sensor pins
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

  // Delare variables for last pump stop time and settling
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

Values to try for initialization, but tune based on soil type, pump flowrate, and pot size
wateringDuration:   3 seconds
settlingDelay:      15 seconds
minPumpOffTime:     8–10 seconds
wateringCooldown:   1 hour
*/
/*
TODO: Verify that the checkInterval is an apporpriate time.
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























