#include <catch_amalgamated.hpp>
#include "irrigation_controller.h"

TEST_CASE("createRuntime initializes all PlantRuntime fields to default values", "[createRuntime]") {
    PlantRuntime runtime = createRuntime();

    REQUIRE(runtime.soilMoisturePercentage == Catch::Approx(0.0f));
    REQUIRE(runtime.moistureLevel == 0);
    REQUIRE(runtime.lastCheckTime == 0u);
    REQUIRE(runtime.pumpStartTime == 0u);
    REQUIRE(runtime.lastWaterTime == 0u);
    REQUIRE(runtime.pumpRunning == false);
    REQUIRE(runtime.lastPumpStopTime == 0u);
    REQUIRE(runtime.settling == false);
}

TEST_CASE("createRuntime returns independent runtime values for each call", "[createRuntime]") {
    PlantRuntime first = createRuntime();
    PlantRuntime second = createRuntime();

    REQUIRE(first.soilMoisturePercentage == Catch::Approx(second.soilMoisturePercentage));
    REQUIRE(first.moistureLevel == second.moistureLevel);
    REQUIRE(first.lastCheckTime == second.lastCheckTime);
    REQUIRE(first.pumpStartTime == second.pumpStartTime);
    REQUIRE(first.lastWaterTime == second.lastWaterTime);
    REQUIRE(first.pumpRunning == second.pumpRunning);
    REQUIRE(first.lastPumpStopTime == second.lastPumpStopTime);
    REQUIRE(first.settling == second.settling);

    first.lastCheckTime = 1234u;
    first.pumpRunning = true;

    REQUIRE(second.lastCheckTime == 0u);
    REQUIRE(second.pumpRunning == false);
}
