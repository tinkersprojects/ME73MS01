#include "ME73MS01.h"

ME73MS01::ME73MS01(Stream &serial) : _serial(&serial), bufferIndex(0) {}

void ME73MS01::begin() {}

bool ME73MS01::available() {
  while (_serial->available()) {
    uint8_t byteIn = _serial->read();

    if (bufferIndex == 0 && byteIn != 0x55) continue;
    if (bufferIndex == 1 && byteIn != 0xA5) {
      bufferIndex = 0;
      continue;
    }

    buffer[bufferIndex++] = byteIn;

    if (bufferIndex >= 18) {
      parseFrame();
      bufferIndex = 0;
      return _target.status != 0;
    }
  }
  return false;
}

RadarTarget ME73MS01::getTarget() {
  return _target;
}

void ME73MS01::parseFrame() {
  _target.status = buffer[8];
  _target.id = buffer[7];
  _target.distance_cm = (buffer[9] << 8) | buffer[10];
  _target.speed_cms = (int16_t)((buffer[11] << 8) | buffer[12]);
  _target.direction = (int8_t)buffer[13];
  _target.pitch = (int8_t)buffer[14];
  _target.strength = (buffer[15] << 8) | buffer[16];
}

uint8_t ME73MS01::checksum(const uint8_t *data, size_t len) {
  uint8_t sum = 0;
  for (size_t i = 0; i < len; i++) sum += data[i];
  return sum;
}

void ME73MS01::sendCommand(uint8_t func, uint8_t cmd1, uint8_t cmd2, const uint8_t *data, uint8_t len) {
  uint8_t frame[10 + len];
  frame[0] = 0x55;
  frame[1] = 0x5A;
  frame[2] = 0x00;
  frame[3] = 4 + len ;
  frame[4] = func;
  frame[5] = cmd1;
  frame[6] = cmd2;
  for (uint8_t i = 0; i < len; i++)
    frame[7 + i] = data[i];

  frame[7 + len] = checksum(frame, 7 + len);

      #ifdef ME_DEBUG
  Serial.print("Tx: ");
  for (uint8_t i = 0; i < (8 + len); i++) {
    if (frame[i] < 0x10) Serial.print("0");
    Serial.print(frame[i], HEX);
    Serial.print(" ");
  }
  Serial.println();
      #endif

  _serial->write(frame, 8 + len);
}

void ME73MS01::sendSimpleCommand(uint8_t func, uint8_t cmd1, uint8_t cmd2, uint16_t val) {
  uint8_t data[2] = { (uint8_t)(val >> 8), (uint8_t)(val & 0xFF) };
  sendCommand(func, cmd1, cmd2, data, 2);
}

uint16_t ME73MS01::readValue(uint8_t cmd1, uint8_t cmd2, uint8_t expectedLen) {
  
  _serial->flush();
  sendCommand(0x00, cmd1, cmd2, nullptr, 0);

  unsigned long start = millis();
  uint8_t reply[8+expectedLen];
  uint8_t index = 0;


      #ifdef ME_DEBUG
  Serial.print("Rx: ");
      #endif
  while (millis() - start < 1000) {
    if (_serial->available()) {

      uint8_t b = _serial->read();

      

      if (index == 0 && b != 0x55) continue;
      if (index == 1 && b != 0xA5) { index = 0; continue; }

      
      #ifdef ME_DEBUG
      
        Serial.print(b < 0x10 ? "0" : "");
        Serial.print(b, HEX);
        Serial.print(" ");
      #endif

      reply[index++] = b;
      if (index >= 8+expectedLen) break;
    }
  }
      #ifdef ME_DEBUG
  Serial.println();
      #endif

  if (index >= 7 && reply[4] == 0x02 && reply[5] == cmd1 && reply[6] == cmd2) {
    if (expectedLen == 10) return (reply[7] << 8) | reply[8];
    if (expectedLen == 9)  return reply[7];
  }
  return 0xFFFF;
}

// === SETTERS ===

void ME73MS01::saveSettings() {
  sendCommand(0x01, 0x20, 0x04, nullptr, 0);
}

void ME73MS01::restoreDefaults() {
  sendCommand(0x01, 0x20, 0x01, nullptr, 0);
}

void ME73MS01::setMotionThreshold(uint16_t value) {
  sendSimpleCommand(0x01, 0x80, 0x03, value);
}

void ME73MS01::setPresenceThreshold(uint16_t value) {
  sendSimpleCommand(0x01, 0x80, 0x09, value);
}

void ME73MS01::setMinDistance(uint16_t cm) {
  sendSimpleCommand(0x01, 0x80, 0x0C, cm);
}

void ME73MS01::setMaxDistance(uint16_t cm) {
  sendSimpleCommand(0x01, 0x80, 0x0E, cm);
}

void ME73MS01::setVOHoldTime(uint16_t ms) {
  sendSimpleCommand(0x01, 0x80, 0x14, ms);
}

void ME73MS01::setVOLevelMode(bool high) {
  uint8_t data[1] = { high ? 0x00 : 0x01 };
  sendCommand(0x01, 0x80, 0x15, data, 1);
}

void ME73MS01::setReportingMode(bool active) {
  uint8_t func = active ? 0x03 : 0x02;
  sendCommand(func, 0x81, 0x00, nullptr, 0);
}

// === GETTERS ===

uint16_t ME73MS01::getSoftwareVersion() {
  return readValue(0x00, 0x01, 14);  // Correct command for version
}

uint16_t ME73MS01::getMotionThreshold() {
  return readValue(0x80, 0x03, 2);  // ✅ FIXED: motion threshold <1m
}

uint16_t ME73MS01::getPresenceThreshold() {
  return readValue(0x80, 0x09, 2);
}

uint16_t ME73MS01::getMinDistance() {
  return readValue(0x80, 0x0C, 2);
}

uint16_t ME73MS01::getMaxDistance() {
  return readValue(0x80, 0x0E, 10);
}

uint16_t ME73MS01::getVOHoldTime() {
  return readValue(0x80, 0x14, 10);
}

uint8_t ME73MS01::getVOLevelMode() {
  return (uint8_t)readValue(0x80, 0x15, 9);
}
//*/