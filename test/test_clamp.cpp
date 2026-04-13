#include <catch_amalgamated.hpp>
#include "irrigation_controller.h"

TEST_CASE("clamp limits values correctly", "[clamp]") {
    REQUIRE(clamp(50, 0, 100) == 50);
    REQUIRE(clamp(-10, 0, 100) == 0);
    REQUIRE(clamp(150, 0, 100) == 100);
}

TEST_CASE("clamp handles boundary values", "[clamp]") {
    REQUIRE(clamp(0, 0, 100) == 0);
    REQUIRE(clamp(100, 0, 100) == 100);
    REQUIRE(clamp(0.0f, 0.0f, 100.0f) == 0.0f);
    REQUIRE(clamp(100.0f, 0.0f, 100.0f) == 100.0f);
}

TEST_CASE("clamp with negative ranges", "[clamp]") {
    REQUIRE(clamp(-50, -100, -10) == -50);
    REQUIRE(clamp(-150, -100, -10) == -100);
    REQUIRE(clamp(0, -100, -10) == -10);
}

TEST_CASE("clamp with floating point precision", "[clamp]") {
    REQUIRE(clamp(1.234567f, 0.0f, 2.0f) == Catch::Approx(1.234567f));
    REQUIRE(clamp(0.000001f, 0.0f, 1.0f) == Catch::Approx(0.000001f));
    REQUIRE(clamp(-0.000001f, 0.0f, 1.0f) == 0.0f);
}

TEST_CASE("clamp with large numbers", "[clamp]") {
    REQUIRE(clamp(1e6f, 0.0f, 1e7f) == 1e6f);
    REQUIRE(clamp(-1e6f, 0.0f, 1e7f) == 0.0f);
    REQUIRE(clamp(1e8f, 0.0f, 1e7f) == 1e7f);
}

TEST_CASE("clamp with zero ranges", "[clamp]") {
    REQUIRE(clamp(0.0f, 0.0f, 0.0f) == 0.0f);
    REQUIRE(clamp(1.0f, 0.0f, 0.0f) == 0.0f);
    REQUIRE(clamp(-1.0f, 0.0f, 0.0f) == 0.0f);
}