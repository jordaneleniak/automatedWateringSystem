#define CATCH_CONFIG_MAIN
//#include "../lib/catch/catch_amalgamated.hpp"
#include <catch_amalgamated.hpp>
#include "../../lib/irrigation_controller/irrigation_controller.h"

TEST_CASE("mapFloat maps values correctly", "[mapFloat]") {
    REQUIRE(mapFloat(0, 0, 100, 0, 1) == Catch::Approx(0.0));
    REQUIRE(mapFloat(50, 0, 100, 0, 1) == Catch::Approx(0.5));
    REQUIRE(mapFloat(100, 0, 100, 0, 1) == Catch::Approx(1.0));
}

TEST_CASE("mapFloat handles inverted ranges", "[mapFloat]") {
    REQUIRE(mapFloat(0, 100, 0, 0, 1) == Catch::Approx(1.0));
}

TEST_CASE("clamp limits values correctly", "[clamp]") {
    REQUIRE(clamp(50, 0, 100) == 50);
    REQUIRE(clamp(-10, 0, 100) == 0);
    REQUIRE(clamp(150, 0, 100) == 100);
}