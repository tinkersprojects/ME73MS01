#ifndef ME73MS01_H
#define ME73MS01_H

#include <Arduino.h>

//#define ME_DEBUG


typedef struct {
  uint8_t id;
  uint8_t status;       // 0: nobody, 1: movement, 2: presence
  uint16_t distance_cm;
  int16_t speed_cms;
  int8_t direction;
  int8_t pitch;
  uint16_t strength;
} RadarTarget;

class ME73MS01 {
public:
  ME73MS01(Stream &serial);

  void begin();

  bool available();
  RadarTarget getTarget();

  // Set parameters
  void saveSettings();
  void restoreDefaults();
  void setMotionThreshold(uint16_t value);
  void setPresenceThreshold(uint16_t value);
  void setMinDistance(uint16_t cm);
  void setMaxDistance(uint16_t cm);
  void setVOHoldTime(uint16_t ms);
  void setVOLevelMode(bool high);
  void setReportingMode(bool active);

  // Get parameters
  uint16_t getSoftwareVersion();
  uint16_t getMotionThreshold();
  uint16_t getPresenceThreshold();
  uint16_t getMinDistance();
  uint16_t getMaxDistance();
  uint16_t getVOHoldTime();
  uint8_t  getVOLevelMode(); //*/

private:
  Stream *_serial;
  RadarTarget _target;
  uint8_t buffer[64];
  uint8_t bufferIndex;

  void parseFrame();
  void sendCommand(uint8_t func, uint8_t cmd1, uint8_t cmd2, const uint8_t *data, uint8_t len);
  void sendSimpleCommand(uint8_t func, uint8_t cmd1, uint8_t cmd2, uint16_t val);
  uint16_t readValue(uint8_t cmd1, uint8_t cmd2, uint8_t expectedLen);
  uint8_t checksum(const uint8_t *data, size_t len); //*/
};

#endif
