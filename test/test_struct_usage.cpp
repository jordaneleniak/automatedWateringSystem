#include <catch_amalgamated.hpp>
#include "irrigation_controller.h"

namespace {
int sampleReadSensor(int pin) {
    (void)pin;
    return 405;
}

void sampleWritePump(int pin, int value) {
    (void)pin;
    (void)value;
}

void sampleLog(const char* message) {
    (void)message;
}
}

TEST_CASE("all related plant structs can be instantiated with realistic values", "[structs]") {
    PlantHardware hardware = {
        3,
        14,
        sampleReadSensor,
        sampleWritePump,
        sampleLog
    };

    SensorCalibration calibration = {
        550,
        275
    };

    PlantConfig config = {
        35,
        65,
        600000UL,
        1500UL,
        3600000UL,
        7000UL,
        12000UL
    };

    PlantRuntime runtime = {
        45.0f,
        409,
        1000UL,
        2000UL,
        3000UL,
        false,
        4000UL,
        false
    };

    PlantSensor plant = createPlant(&hardware, &calibration, &config, &runtime);

    REQUIRE(hardware.PumpPin == 3);
    REQUIRE(hardware.SoilMoistureSensorPin == 14);
    REQUIRE(hardware.readSensor == sampleReadSensor);
    REQUIRE(hardware.writePump == sampleWritePump);
    REQUIRE(hardware.log == sampleLog);

    REQUIRE(calibration.AirValue == 550);
    REQUIRE(calibration.WaterValue == 275);

    REQUIRE(config.lowMoisture == 35);
    REQUIRE(config.highMoisture == 65);
    REQUIRE(config.checkInterval == 600000UL);
    REQUIRE(config.wateringDuration == 1500UL);
    REQUIRE(config.wateringCooldown == 3600000UL);
    REQUIRE(config.minPumpOffTime == 7000UL);
    REQUIRE(config.settlingDelay == 12000UL);

    REQUIRE(runtime.soilMoisturePercentage == Catch::Approx(45.0f));
    REQUIRE(runtime.moistureLevel == 409);
    REQUIRE(runtime.lastCheckTime == 1000UL);
    REQUIRE(runtime.pumpStartTime == 2000UL);
    REQUIRE(runtime.lastWaterTime == 3000UL);
    REQUIRE(runtime.pumpRunning == false);
    REQUIRE(runtime.lastPumpStopTime == 4000UL);
    REQUIRE(runtime.settling == false);

    REQUIRE(plant.hardware == &hardware);
    REQUIRE(plant.calibration == &calibration);
    REQUIRE(plant.config == &config);
    REQUIRE(plant.runtime == &runtime);
}
