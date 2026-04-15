# Automated Watering System – GitHub Issues (Copy/Paste)

## VERSION 1.0.0 — DEPLOYABLE SYSTEM

### [TEST] Testing & Validation (Parent)
Labels: test
Milestone: v1.0.0
Priority: High

Acceptance Criteria:
- All sub-issues completed
- Core logic validated
- Stable behavior confirmed

---

#### [TEST] Expand unit test coverage for irrigation_controller
Parent: Testing & Validation
Labels: test
Priority: High

Acceptance Criteria:
- creation logic tested
- runtime initialization tested
- updatePlant behavior tested

---

#### [TEST] Validate mapFloat edge cases
Parent: Testing & Validation
Labels: test
Priority: High

Acceptance Criteria:
- inverted ranges tested
- boundary values tested
- division by zero handled

---

#### [TEST] Validate clamp edge cases
Parent: Testing & Validation
Labels: test
Priority: High

Acceptance Criteria:
- min/max bounds tested
- equal bounds tested
- out-of-range inputs tested

---

#### [TEST] Test timing logic
Parent: Testing & Validation
Labels: test
Priority: High

Acceptance Criteria:
- cooldown timing verified
- settling delay verified
- minimum off-time verified

---

#### [TEST] Create structured test plan
Parent: Testing & Validation
Labels: test
Priority: Medium

---

#### [TEST] Simulate moisture signals
Parent: Testing & Validation
Labels: test
Priority: Medium

---

#### [TEST] Verify system behavior
Parent: Testing & Validation
Labels: test
Priority: High

---

#### [TEST] Add serial logging for debugging
Parent: Testing & Validation
Labels: test
Priority: Medium

---

#### [TEST] Long-duration stability test
Parent: Testing & Validation
Labels: test
Priority: High

---

#### [TEST] Sensor failure handling tests
Parent: Testing & Validation
Labels: test
Priority: High

---

### [BUG] Bugs (Parent)
Labels: bug
Milestone: v1.0.0
Priority: High

---

#### [BUG] mapFloat incorrect behavior with inverted ranges
Parent: Bugs
Labels: bug
Priority: High

---

### [REFACTOR] Code Improvements (Parent)
Labels: refactor
Milestone: v1.0.0
Priority: Medium

---

#### [REFACTOR] Separate logic and hardware layers
Parent: Code Improvements
Labels: refactor
Priority: High

---

#### [REFACTOR] Improve control logic readability
Parent: Code Improvements
Labels: refactor
Priority: Medium

---

#### [REFACTOR] Modularize architecture
Parent: Code Improvements
Labels: refactor
Priority: Medium

---

### [FEATURE] Core System Features (Parent)
Labels: feature
Milestone: v1.0.0
Priority: High

---

#### [FEATURE] Perform real-world validation
Parent: Core System Features
Labels: feature
Priority: High

---

#### [FEATURE] Define calibration procedure
Parent: Core System Features
Labels: feature
Priority: High

---

#### [FEATURE] Basic power setup
Parent: Core System Features
Labels: feature
Priority: Medium

---

#### [FEATURE] Sensor failure detection and handling
Parent: Core System Features
Labels: feature
Priority: High

---

#### [FEATURE] Add pump runtime safety limits
Parent: Core System Features
Labels: feature
Priority: High

---

#### [FEATURE] Prevent oscillation near threshold boundaries
Parent: Core System Features
Labels: feature
Priority: High

---

### [DOCS] Documentation (Parent)
Labels: docs
Milestone: v1.0.0
Priority: Low

---

#### [DOCS] Create wiring diagram
Parent: Documentation
Labels: docs

---

#### [DOCS] Collect component documentation
Parent: Documentation
Labels: docs

---

#### [DOCS] Improve code comments
Parent: Documentation
Labels: docs

---

## VERSION 2.0.0 — FUTURE WORK

### [FEATURE] Architecture Improvements (Parent)
Labels: feature
Milestone: v2.0.0

---

#### [FEATURE] Convert to state machine
Parent: Architecture Improvements

---

#### [FEATURE] Define full state transition table
Parent: Architecture Improvements

---

### [FEATURE] Platform Expansion (Parent)
Labels: feature
Milestone: v2.0.0

---

#### [FEATURE] STM32 implementation
Parent: Platform Expansion

---

#### [FEATURE] ESP32 implementation
Parent: Platform Expansion

---

### [FEATURE] Power System (Parent)
Labels: feature
Milestone: v2.0.0

---

#### [FEATURE] Battery backup system
Parent: Power System

---

### [FEATURE] Hardware Improvements (Parent)
Labels: feature
Milestone: v2.0.0

---

#### [FEATURE] Custom PCB design
Parent: Hardware Improvements

---

#### [FEATURE] Enclosure design
Parent: Hardware Improvements

---

### [FEATURE] Smart Features (Parent)
Labels: feature
Milestone: v2.0.0

---

#### [FEATURE] RTC / NTP integration
Parent: Smart Features

---

#### [FEATURE] Seasonal watering logic
Parent: Smart Features

---

#### [FEATURE] UI / Display system
Parent: Smart Features

---

#### [FEATURE] Remote monitoring dashboard
Parent: Smart Features
