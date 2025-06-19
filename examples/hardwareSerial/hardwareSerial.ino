#include <ME73MS01.h>

// Radar module connected to Serial1 on ESP32 (RX=34, TX=13)
ME73MS01 radar(Serial1);

void setup() {
  Serial.begin(115200);
  delay(1000);

  // Initialize radar UART
  Serial1.begin(115200, SERIAL_8N1, 34, 13); // ESP32 Serial1 RX, TX
  radar.begin();

  Serial.println("ME73MS01 Radar Initialized");
  
  //radar.getMotionThreshold();
  //radar.restoreDefaults();
  // Optional: configure radar thresholds (e.g., set min detection distance)
   //radar.setMinDistance(00);   // 100 cm
   //radar.setMaxDistance(100);   // 500 cm
   //radar.setPresenceThreshold(100);   // 500 cm
}

void loop() {

  if (radar.available()) {
    RadarTarget target = radar.getTarget();

    if (target.status != 0) {
      Serial.print(" "); Serial.print(target.id);
      Serial.print(","); Serial.print(target.status );
      Serial.print(","); Serial.print(target.distance_cm);
      Serial.print(","); Serial.print(target.strength);

/*
      uint16_t getVOHoldTime = radar.getVOHoldTime();
      uint16_t getVOLevelMode = radar.getVOLevelMode();
      
      Serial.print(","); Serial.print(getVOHoldTime);
      Serial.print(","); Serial.println(getVOLevelMode);*/
    }
  }

}
