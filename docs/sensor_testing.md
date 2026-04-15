Get the voltage reading from capacitive soil moisture sensor
Note: This is not used in control logic, but may be useful for testing,
debugging, and calibration (e.g., when using a function generator).

The sensor outputs approximately 0–3.3V. The ADC reads this as a raw value
(rawMoistureLevel), which depends on the microcontroller’s ADC resolution
and reference voltage.

In this system, calibration values are defined:
- airValue   ≈ 675 → dry soil / air (higher ADC value)
- waterValue ≈ lower value → wet soil

The control logic uses calibrated percentage (0–100%), NOT voltage.
However, voltage can be estimated in two ways:

1) General ADC-based conversion (portable across platforms):
   float voltage = rawMoistureLevel * (Vref / ADC_MAX);

   where:
   - Vref = ADC reference voltage (e.g., 5.0V for Arduino UNO, 3.3V for STM32/ESP32)
   - ADC_MAX = maximum ADC value (UNO: 1023, STM32/ESP32: 4095)

2) Calibration-based approximation (based on dry value):
   float voltage = rawMoistureLevel * (Vref / airValue);

   Example (using calibrated dry value ≈ 675):
   float voltage = rawMoistureLevel * (3.3 / 675);

   This assumes airValue corresponds to ~3.3V and is less portable,
   but can be useful for debugging relative sensor behavior.

Summary:
- Use percentage for control logic
- Use voltage only for testing, debugging, and calibration insight