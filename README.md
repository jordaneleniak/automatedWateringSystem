# Automated Watering System

An Arduino-based automated plant watering system that monitors soil moisture and dynamically controls water pumps to maintain optimal soil conditions.

## Overview

This project implements a non-blocking irrigation controller using `millis()` timing. It uses burst watering with settling delays to improve measurement accuracy and prevent overwatering.

The system is designed to be scalable and supports multiple plants while maintaining accurate readings and stable soil hydration. It is also structured to enable future migration to STM32 and ESP32 platforms, as well as additional features such as WiFi monitoring and seasonal watering logic.

The system operates using burst watering cycles with settling delays to ensure accurate moisture readings and stable soil hydration.

---

## Motivation

This project was developed to explore embedded system design, non-blocking control logic, and real-world sensor integration for automated plant care.

---

## Architecture

The system is structured with separation between hardware configuration, calibration data, runtime state, and control logic. This allows the core watering logic to be portable across different platforms (Arduino, STM32, ESP32).

### Architecture Direction

The system is being refactored into two layers:

- **Core Logic Layer**
  - Platform-independent
  - Contains watering logic, timing rules, and state decisions
  - Unit tested on PC

- **Hardware Layer**
  - Platform-specific (Arduino, STM32, ESP32)
  - Handles ADC, GPIO, and timing functions

This layered design improves testability, portability, and long-term maintainability.

---

## Design Principles

- Non-blocking control using `millis()`
- Separation of logic and hardware layers
- Testable design with unit, component, and integration testing across software and hardware layers
- Scalability for multi-plant systems
- Fault tolerance (safe startup, future persistence)

---

## Known Limitations

- No persistent state storage implemented yet
- No RTC/NTP integration (timing resets on power loss)
- Hardware validation not yet completed

---

## Testing Strategy

This project follows a layered testing approach:

- **Unit Testing (PC)**
  - Core logic is refactored into platform-independent functions
  - Tested using native C++ frameworks (Using Catch2 (native C++) for unit testing)

- **Embedded Testing (Arduino)**
  - Uses AUnit to validate timing behavior and hardware interaction

- **Hardware Validation**
  - Performed after unit and integration testing
  - Uses Digilent Analog Discovery 2 to simulate sensor signals and verify system response

To support this, logic is being separated from hardware-specific code (e.g., `millis()`, `digitalWrite()`).

---

## Features

- Soil moisture calibration (air/water reference values)
- Burst watering with configurable duration
- Settling delay between watering cycles for accurate readings
- Minimum pump-off time to prevent rapid cycling and hardware damage
- Cooldown period to prevent overwatering
- Non-blocking timing using `millis()`
- Modular and scalable design for multiple plants

---

## How It Works

1. Periodically reads soil moisture
2. Converts raw sensor values to percentage using calibration
3. If soil moisture falls below the configured lower threshold:
   - Starts a watering burst
4. After watering:
   - Waits for a settling delay
   - Rechecks moisture
5. Stops watering when:
   - Moisture reaches upper threshold, or
   - Burst duration completes
6. Enforces:
   - Pump protection timing
   - Cooldown period

> Thresholds are defined relative to calibrated sensor values and approximate soil moisture ranges (e.g., between dry and near field capacity conditions for the specific plant).

---

## Hardware

- Arduino UNO (current platform)
- Capacitive soil moisture sensor(s)
- 4-channel 5V relay module
- Water pump(s)

> Planned: STM32 migration for enhanced embedded control and ESP32-S3 migration for connectivity and advanced features

---

## Project Structure

```
Arduino/
└── sketch_plantWateringAutomation/
    └── sketch_plantWateringAutomation.ino

STM32/
└── plantWateringAutomation/
    └── (STM32CubeIDE project files)

ESP32/
└── (planned implementation)

lib/                               # Shared logic + test framework (Catch2)
├── irrigation_controller/
│   ├── irrigation_controller.cpp
│   ├── irrigation_controller.h
│   └── ic_main.cpp.disable        # Disabled during unit testing to avoid multiple entry points (main function conflict)
└── catch/
    ├── catch_amalgamated.cpp
    └── catch_amalgamated.hpp

test/                               # Unit tests (Catch2, native environment)
├── staging/                        # These tests are under development and are not yet included in the main test runner (test_main.cpp)
│   ├── creation/
│   │   ├── test_createPlant.cpp
│   │   └── test_createRuntime.cpp
│   ├── logic/
│   │   ├── test_readMoisture.cpp
│   │   ├── test_runPump.cpp
│   │   └── test_updatePlant.cpp
│   ├── mock/
│   │   ├── mock_hardware.cpp
│   │   └── mock_hardware.h
│   ├── state/
│   ├── timing/
│   │   ├── test_cooldown.cpp
│   │   └── test_pumpTiming.cpp
├── test_main.cpp                   # Active test runner
├── test_clamp.cpp
└── test_mapFloat.cpp

docs/
```

---

## Current Status

**Version:** 0.7.0  (Development Phase)
**State:** Functional and stable core logic implemented
**Focus:** Expanding test coverage and validating system behavior

### Completed
- Non-blocking timing system
- Burst watering logic
- Settling delay implementation
- Pump protection (minimum off-time)
- Cooldown logic
- Struct-based modular design

---

## 🔢 Version Goals

This project follows a structured versioning approach where each major version represents a meaningful increase in system reliability and capability.

### 🟡 Version 0.x — Development Phase
- Core functionality is being implemented and refined
- Architecture is evolving (modularization, testing, refactoring)
- Unit testing and validation are in progress
- Breaking changes may occur

---

### 🟢 Version 1.0.0 — Deployable Arduino System
Goal: A reliable, fully functional watering system that can be used in real-world conditions.

Planned features:
- Stable moisture-based watering logic
- Calibration process for different plants/environments
- Reliable timing and control behavior
- Arduino-based implementation (UNO)
- Powered via wall adapter (barrel jack or USB)
- Simple, reproducible hardware setup (no custom PCB required)
- Verified through unit, component, and real-world testing

---

### 🔵 Version 2.0.0 — Expanded & Enhanced System
Goal: Improve flexibility, efficiency, and system design.

Planned features:
- State machine-based architecture
- Multi-platform support (STM32, ESP32, etc.)
- Advanced power system:
  - Wall power + battery backup
  - Power source switching
  - Low-power operation mode
- Custom PCB design
- Optional enclosure (case design)
- Increased modularity and system scalability

---

## Getting Started

1. Clone the repository:
   ```bash
   git clone https://github.com/jordaneleniak/automatedWateringSystem.git
   ```

2. Open the Arduino sketch:
   ```
   Arduino/sketch_plantWateringAutomation/sketch_plantWateringAutomation.ino
   ```

3. Upload to your Arduino board

4. Connect hardware according to pin configuration in the code

---

## Notes

- Calibration values (`AirValue`, `WaterValue`) must be adjusted for each sensor
- Watering parameters should be tuned based on:
  - soil type
  - pot size
  - pump flow rate

---

## Author

Jordan Eleniak
