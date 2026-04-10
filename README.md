# Automated Watering System

An Arduino-based automated plant watering system that monitors soil moisture and dynamically controls water pumps to maintain optimal soil conditions.

## Overview

This project implements a non-blocking irrigation controller using `millis()` timing. It uses burst watering with settling delays to improve measurement accuracy and prevent overwatering.

The system is designed to be scalable and supports multiple plants. It is also structured to enable future migration to STM32 and ESP32 platforms, as well as additional features such as WiFi monitoring and seasonal watering logic.

The system operates using burst watering cycles with settling delays to ensure accurate moisture readings and stable soil hydration.

---

## Motivation

This project was developed to explore embedded system design, non-blocking control logic, and real-world sensor integration for automated plant care.

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

## Architecture

The system is structured with separation between hardware configuration, calibration data, runtime state, and control logic. This allows the core watering logic to be portable across different platforms (Arduino, STM32, ESP32).

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

### High Priority
- Convert control logic to state machine (IDLE, WATERING, SETTLING, COOLDOWN)

### Medium Priority
- Add RTC/NTP for time-based watering
- Implement seasonal watering profiles

### Future Enhancements
- ESP32 migration
- WiFi monitoring dashboard
- OLED/LCD display
- Code refactoring into header/source files

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
