# ME73MS01 Library

[https://tinkersprojects.com/](https://tinkersprojects.com/)  

This library enables communication with the **ME73MS01** millimeter-wave radar sensor over UART using either **HardwareSerial** or **SoftwareSerial**.

## Features

- Compatible with **ESP32**, **Arduino UNO/Nano**, and other boards
- Supports both **active** and **passive** reporting modes
- Easily set and read radar parameters:
  - Motion threshold
  - Presence threshold
  - Minimum and maximum detection range
  - VO output settings
- Retrieve target information (distance, strength, speed, direction, etc.)
- EEPROM save/restore functionality

---

## Supported Boards

- ✅ Arduino UNO / Nano / Mega (via `SoftwareSerial`)
- ✅ ESP32 (via `Serial1` or other `HardwareSerial`)

---

## Installation

1. Copy the `ME73MS01.h` and `ME73MS01.cpp` files to your Arduino `libraries/ME73MS01/` folder.
2. Restart the Arduino IDE.
3. Use one of the examples provided to get started.

---

## Basic Usage

### ESP32 Example (HardwareSerial)
```cpp
#include <ME73MS01.h>
ME73MS01 radar(Serial1);

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200, SERIAL_8N1, 34, 13);  // RX, TX
  radar.begin();
}

void loop() {
  if (radar.available()) {
    RadarTarget t = radar.getTarget();
    Serial.println(t.distance_cm);
  }
}
````

### Arduino UNO Example (SoftwareSerial)

```cpp
#include <ME73MS01.h>
#include <SoftwareSerial.h>

SoftwareSerial radarSerial(10, 11); // RX, TX
ME73MS01 radar(radarSerial);

void setup() {
  Serial.begin(115200);
  radarSerial.begin(115200);
  radar.begin();
}

void loop() {
  if (radar.available()) {
    RadarTarget t = radar.getTarget();
    Serial.println(t.distance_cm);
  }
}
```

---

## API Reference

### Initialization

```cpp
ME73MS01 radar(Stream &serial);
void begin();
```

### Target Data

```cpp
bool available();             // Returns true if valid data was received
RadarTarget getTarget();     // Returns struct with:
                             // id, status, distance_cm, speed_cms,
                             // direction, pitch, strength
```

### Configuration Commands

```cpp
void setMotionThreshold(uint16_t value);
void setPresenceThreshold(uint16_t value);
void setMinDistance(uint16_t cm);
void setMaxDistance(uint16_t cm);
void setVOHoldTime(uint16_t ms);
void setVOLevelMode(bool high);         // true = high level active
void setReportingMode(bool active);     // true = active, false = passive

void saveSettings();
void restoreDefaults();
```

### Read Parameters

```cpp
uint16_t getMotionThreshold();
uint16_t getPresenceThreshold();
uint16_t getMinDistance();
uint16_t getMaxDistance();
uint16_t getVOHoldTime();
uint8_t  getVOLevelMode();
```

---

## License

MIT License

---

## Notes

* When using `SoftwareSerial`, avoid fast polling or multiple sensors on the same port.
* The default baud rate for the radar module is **115200**.
* Always call `begin()` after starting your serial port.

```

---

Let me know if you'd like it split into a `docs/` folder or zipped with examples included.
```
