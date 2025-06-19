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
    RadarTarget target = radar.getTarget();
    Serial.print("ID: "); Serial.print(target.id);
    Serial.print(" | Status: "); Serial.print(target.status);
    Serial.print(" | Distance: "); Serial.print(target.distance_cm);
    Serial.print(" cm | Signal: "); Serial.println(target.strength);
  }
}
