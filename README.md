# Automated Watering System

An Arduino-based automated plant watering system that monitors soil moisture and dynamically controls water pumps to maintain optimal soil conditions.

## Overview

This project implements a non-blocking irrigation controller using `millis()` timing. It uses burst watering with settling delays to improve measurement accuracy and prevent overwatering.

The system is designed to be scalable and is structured to support multiple plants, future ESP32 migration, and additional features such as WiFi monitoring and seasonal watering logic.

The system operates using burst watering cycles with settling delays to ensure accurate moisture readings and stable soil hydration.

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

> Planned: ESP32-S3 migration for WiFi and advanced features

---

## Project Structure

```
Arduino/
└── sketch_plantWateringAutomation.ino
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
   Arduino/sketch_plantWateringAutomation.ino
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
