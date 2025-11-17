#include "gps_sd.h"
#include <Arduino.h>
#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>
#include <SD.h>

TinyGPS gps;
SoftwareSerial ssgps(17, 16);

File file;
const byte CS_PIN = 29;
const char* FILE_NAME = "gps_data.csv";

void gps_sd_init() {
  ssgps.begin(9600);
  if (!SD.begin(CS_PIN)) {
    Serial.println("SD init failed");
  }
  file = SD.open(FILE_NAME, FILE_WRITE);
  if (!file) {
    Serial.println("Error opening file");
  }
}

void gps_sd_update(double sensors_data[3]) {
  while (ssgps.available()) {
    gps.encode(ssgps.read());
  }

  if (gps.location.isValid()) {
    static unsigned long last_save = 0;
    if (millis() - last_save >= 60000) {
      last_save = millis();
      file.print(gps.location.lat(), 6);
      file.print(",");
      file.print(gps.location.lng(), 6);
      file.print(",");
      file.print(gps.speed.kmph());
      file.print(",");
      file.print(gps.altitude.meters());
      file.print(",");
      file.print(sensors_data[0]);
      file.print(",");
      file.print(sensors_data[1]);
      file.print(",");
      file.println(sensors_data[2]);
      file.flush();
    }

    // Отладочный вывод
      Serial.print(gps.location.lat(), 6);
      Serial.print(",");
      Serial.print(gps.location.lng(), 6);
      Serial.print(",");
      Serial.print(gps.speed.kmph());
      Serial.print(",");
      Serial.print(gps.altitude.meters());
      Serial.print(",");
      Serial.print(sensors_data[0]);
      Serial.print(",");
      Serial.print(sensors_data[1]);
      Serial.print(",");
      Serial.println(sensors_data[2]);
  } else {
    Serial.println("No GPS data");
  }
}
