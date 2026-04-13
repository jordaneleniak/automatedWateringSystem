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

This separation enables portability and easier testing.

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
3. If soil is too dry:
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
└── test_math.cpp                   # Core logic tests

docs/
```

---

## Current Status

**Version:** 0.5  
**State:** Functional and stable core logic implemented

### Completed
- Non-blocking timing system
- Burst watering logic
- Settling delay implementation
- Pump protection (minimum off-time)
- Cooldown logic
- Struct-based modular design

---

## TODO / Roadmap

### Platform Roadmap

This project is being developed across multiple platforms:

- Arduino UNO – initial prototyping and validation
- STM32 (L432KC) – embedded system implementation with finer control
- ESP32 – future expansion with WiFi, NTP, and user interface features

### Current Focus: Testing & Validation

- Expand unit test coverage (Catch2)
  - Test core math/utilities (mapFloat, clamp) ✅
  - Test plant creation and initialization logic
  - Test moisture reading and percentage conversion
  - Test pump control logic (start/stop conditions)
  - Test timing logic (cooldown, settling, min off time)
- Validate logic against edge cases
  - Rapid moisture fluctuations
  - Boundary thresholds (exact low/high values)
  - Timing edge cases (overflow-safe comparisons)

---

### Next Phase: Control Logic Improvements

- Add documentation and comments for improved readability
- Convert control logic to state machine:
  - Lifecycle states: UNINITIALIZED, INITIALIZING, READY
  - Operational states: IDLE, WATERING, SETTLING, COOLDOWN
  - Events/actions:
    - RESET (forces transition to UNINITIALIZED)
- Add helper functions for clarity (e.g., `cooldownExpired`, `settlingComplete`)
- Improve readability and maintainability of control flow
- Refactor into modular components (architectural decomposition):
  - Separate plant logic, control logic, and hardware interfaces
  - Organize into dedicated header/source files

---

### Platform Development

- Complete STM32 bring-up
  - Configure ADC and GPIO
  - Implement HAL-based timing
  - Port and validate core logic

- Implement ESP32 version
  - Validate ADC differences
  - Add WiFi capability
  - Prepare for remote monitoring

---

### Code Organization

- Finalize separation of logic and hardware layers
- Expand reusable core logic module
- Add logging/debug framework for testing and validation

---

### Hardware & Reliability

- Add pump runtime safety limits (max runtime per hour)
- Add reservoir level sensor support
- Add sensor failure detection and handling
- Validate relay and pump behavior under real load

---

### Calibration & Tuning

- Define calibration procedure (AirValue, WaterValue)
- Validate ADC → percentage mapping
- Tune:
  - watering duration
  - settling delay
  - cooldown timing

---

### Time & Smart Features

- Integrate RTC or NTP time source for time-based watering
- Implement seasonal watering profiles
- Adjust watering behavior based on environment (time/temp)

---

### Monitoring & UI

- Add OLED/LCD display
- Display:
  - moisture level
  - system state
  - last watering time
- Implement WiFi dashboard (ESP32)

---

### State Management (Planned)

Future versions will include:

- Plant initialization phase (baseline moisture detection)
- Per-plant reset capability (for sensor reassignment)
- Persistent state storage (EEPROM / RTC / NTP)
- Safe startup handling after power loss

These features will be implemented as part of the state machine design.

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
