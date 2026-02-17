// Declare and initialize constant variables for air and water for mositure percentage calculations
//const int AirValue = 564;
//const int WaterValue = 271;
// float voltage1 = moistureLevel1 * (3.3 / 675); // Get the voltage reading from capacitive soil moisture sensor

struct PlantSensor {
  const int AirValue;
  const int WaterValue;
  const int PumpPin;
  const int SoilMoistureSensorPin;
  float soilMoisturePercentage;
  float moistureLevel;
  int lowMoisture;
  int highMoisture;
  unsigned long delayTime;
  unsigned long checkInterval;
  unsigned long lastCheckTime;
};

// Declare and initialize variables for soil moisture percentages
  float soilMoisturePercentage1 = 0.0;

// Declare and initialize variables for pins for pumps
int pump1Pin = 2;

// Declare and initialize variables for pins for capacitive soil moisture sensors
int soilMoistureSensor1Pin = A0;

// Declare and initialize variables for moisture levels
float moistureLevel1 = 0.0;

// Declare and initialize PlantSensor struct, including pins for pumps and capacitive soil moisture sensors, soil moisture percentages, and moisture levels 
/*
Pins for future plant sensors:
Pump pins: 3, 4, 5
Soil Moisture Sensor pins: A1, A2, A3
*/
PlantSensor plants[] = {
  {
  .AirValue = 564,
  .WaterValue = 271,
  .PumpPin = 2,
  .SoilMoistureSensorPin = A0,
  .soilMoisturePercentage = 0.0,
  .moistureLevel = 0.0,
  .lowMoisture = 35,
  .highMoisture = 65,
  .delayTime = 10000,
  .checkInterval = 3600000,
  .lastCheckTime = 0}
};
const int arrayLength = sizeof(plants) / sizeof(plants[0]);


// Function to toggle pump on or off based on plant soil moisture
void togglePump(PlantSensor& plantSensor) {
  plantSensor.moistureLevel = analogRead(plantSensor.SoilMoistureSensorPin);
  plantSensor.soilMoisturePercentage = (float)map(plantSensor.moistureLevel, plantSensor.AirValue, plantSensor.WaterValue, 0, 100);
  Serial.print("Raw sensor moisture level: ");
  Serial.println(plantSensor.moistureLevel);
  if (plantSensor.soilMoisturePercentage >= plantSensor.highMoisture) {
    // Wet soil
    Serial.print("Moisture Level of plant: ");
    Serial.print(plantSensor.soilMoisturePercentage);
    Serial.println("%");
  } else if (plantSensor.soilMoisturePercentage <= plantSensor.lowMoisture) {
    // Dry soil
    Serial.print("Moisture Level of plant: ");
    Serial.print(plantSensor.soilMoisturePercentage);
    Serial.println("%");
    // Engage pump to water soil
    Serial.println("Start watering");
    digitalWrite(plantSensor.PumpPin, HIGH);
    delay(plantSensor.delayTime);
    digitalWrite(plantSensor.PumpPin, LOW);
    Serial.println("Watering stopped");
  } else {
    // Moist soil
    Serial.print("Moisture Level of plant: ");
    Serial.print(plantSensor.soilMoisturePercentage);
    Serial.println("%");
  }
}


void setup() {
  // put your setup code here, to run once:
  
  // Set the data rate in bits per second (baud) for serial data transmission
  Serial.begin(9600);
  
    // Initialize pinmodes for water pumps and capacitive soil moisture sensors, and digital writes to set high/low values to digital pins
  for (int i = 0; i < arrayLength; i++) {
    pinMode(plants[i].PumpPin, OUTPUT);
    pinMode(plants[i].SoilMoistureSensorPin, INPUT);
    digitalWrite(plants[i].PumpPin, LOW);
  }

//   Initialize pinmodes for water pumps
//  pinMode(pump1Pin, OUTPUT);

  // Initialize pinmodes for capacitive soil moisture sensors
//  pinMode(soilMoistureSensor1Pin, INPUT);

  // Initialize digitial writes to  set high/low values to digital pins
//  digitalWrite(pump1Pin, LOW);

  // Pause program for a half second before re-running
  delay(500);
}

void loop() {
  // put your main code here, to run repeatedly:
  // Toggle pump on or off based on plant soil moisture
  for (int i = 0; i < arrayLength; i++) {
    togglePump(plants[i]);
  }
  /*
  // Toggle pump 1 on or off based on plant soil moisture
  moistureLevel1 = analogRead(soilMoistureSensor1Pin);
  soilMoisturePercentage1 = map(moistureLevel1, AirValue, WaterValue, 0, 100);
  Serial.print(moistureLevel1);
  if(soilMoisturePercentage1 >= 65) {
    // Wet soil
    Serial.print("Moisture Level of plant 1: ");
    Serial.print(soilMoisturePercentage1);
    Serial.println("%");
  } else if (soilMoisturePercentage1 <= 35) {
    // Dry soil
    Serial.print("Moisture Level of plant 1: ");
    Serial.print(soilMoisturePercentage1);
    Serial.println("%");
    // Engage pump to water soil
    Serial.print("Start watering");
    digitalWrite(pump1Pin, HIGH);
    delay(10000); // TODO: Change value to a set time to water plant
    digitalWrite(pump1Pin, LOW);
    Serial.print("Watering stopped");
  } else {
    // Moist soil
    Serial.print("Moisture Level of plant 1: ");
    Serial.print(soilMoisturePercentage1);
    Serial.println("%");
  }
  */
/*
  // Toggle pump 2 on or off based on plant soil moisture
  Serial.print("Moisture Level of plant 2:");
  moistureLevel2 = analogRead(soilMoistureSensor2Pin);
  Serial.println(moistureLevel2);
  if(moistureLevel2>550) {
    digitalWrite(pump2Pin, LOW);
  } else {
    digitalWrite(pump2Pin, HIGH);
  }

  // Toggle pump 3 on or off based on plant soil moisture
  Serial.print("Moisture Level of plant 3:");
  moistureLevel3 = analogRead(soilMoistureSensor3Pin);
  Serial.println(moistureLevel3);
  if(moistureLevel3>550) {
    digitalWrite(pump3Pin, LOW);
  } else {
    digitalWrite(pump3Pin, HIGH);
  }

  // Toggle pump 4 on or off based on plant soil moisture
  Serial.print("Moisture Level of plant 4:");
  moistureLevel4 = analogRead(soilMoistureSensor4Pin);
  Serial.println(moistureLevel4);
  if(moistureLevel4>550) {
    digitalWrite(pump4Pin, LOW);
  } else {
    digitalWrite(pump4Pin, HIGH);
  }
*/
  // Delay for 1 hour, value in milliseconds
  //delay(3600000);

  delay(1000);
}

























