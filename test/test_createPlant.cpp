#include <catch_amalgamated.hpp>
#include "irrigation_controller.h"

namespace {
int mockReadSensor(int pin) {
    (void)pin;
    return 512;
}

void mockWritePump(int pin, int value) {
    (void)pin;
    (void)value;
}

void mockLog(const char* message) {
    (void)message;
}
}

TEST_CASE("createPlant links all provided structs into a PlantSensor", "[createPlant]") {
    PlantHardware hardware = {
        9,
        2,
        mockReadSensor,
        mockWritePump,
        mockLog
    };

    SensorCalibration calibration = {
        550,
        275
    };

    PlantConfig config = {
        30,
        70,
        3600000UL,
        3000UL,
        3600000UL,
        8000UL,
        15000UL
    };

    PlantRuntime runtime = createRuntime();

    PlantSensor plant = createPlant(&hardware, &calibration, &config, &runtime);

    REQUIRE(plant.hardware == &hardware);
    REQUIRE(plant.calibration == &calibration);
    REQUIRE(plant.config == &config);
    REQUIRE(plant.runtime == &runtime);

    REQUIRE(plant.hardware->PumpPin == 9);
    REQUIRE(plant.hardware->SoilMoistureSensorPin == 2);
    REQUIRE(plant.hardware->readSensor == mockReadSensor);
    REQUIRE(plant.hardware->writePump == mockWritePump);
    REQUIRE(plant.hardware->log == mockLog);

    REQUIRE(plant.calibration->AirValue == 550);
    REQUIRE(plant.calibration->WaterValue == 275);

    REQUIRE(plant.config->lowMoisture == 30);
    REQUIRE(plant.config->highMoisture == 70);
    REQUIRE(plant.config->checkInterval == 3600000UL);
    REQUIRE(plant.config->wateringDuration == 3000UL);
    REQUIRE(plant.config->wateringCooldown == 3600000UL);
    REQUIRE(plant.config->minPumpOffTime == 8000UL);
    REQUIRE(plant.config->settlingDelay == 15000UL);

    REQUIRE(plant.runtime->soilMoisturePercentage == Catch::Approx(0.0f));
    REQUIRE(plant.runtime->moistureLevel == 0);
    REQUIRE(plant.runtime->lastCheckTime == 0u);
    REQUIRE(plant.runtime->pumpStartTime == 0u);
    REQUIRE(plant.runtime->lastWaterTime == 0u);
    REQUIRE(plant.runtime->pumpRunning == false);
    REQUIRE(plant.runtime->lastPumpStopTime == 0u);
    REQUIRE(plant.runtime->settling == false);
}

TEST_CASE("createPlant preserves an existing runtime instance rather than replacing it", "[createPlant]") {
    PlantHardware hardware = {
        5,
        34,
        mockReadSensor,
        mockWritePump,
        mockLog
    };

    SensorCalibration calibration = {
        600,
        250
    };

    PlantConfig config = {
        20,
        80,
        1800000UL,
        2000UL,
        3600000UL,
        5000UL,
        10000UL
    };

    PlantRuntime runtime = createRuntime();
    runtime.soilMoisturePercentage = 42.5f;
    runtime.moistureLevel = 420;
    runtime.lastCheckTime = 1111u;
    runtime.pumpStartTime = 2222u;
    runtime.lastWaterTime = 3333u;
    runtime.pumpRunning = true;
    runtime.lastPumpStopTime = 4444u;
    runtime.settling = true;

    PlantSensor plant = createPlant(&hardware, &calibration, &config, &runtime);

    REQUIRE(plant.runtime == &runtime);
    REQUIRE(plant.runtime->soilMoisturePercentage == Catch::Approx(42.5f));
    REQUIRE(plant.runtime->moistureLevel == 420);
    REQUIRE(plant.runtime->lastCheckTime == 1111u);
    REQUIRE(plant.runtime->pumpStartTime == 2222u);
    REQUIRE(plant.runtime->lastWaterTime == 3333u);
    REQUIRE(plant.runtime->pumpRunning == true);
    REQUIRE(plant.runtime->lastPumpStopTime == 4444u);
    REQUIRE(plant.runtime->settling == true);
}
