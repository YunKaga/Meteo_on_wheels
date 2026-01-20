#include "gps_sd.h"
#include "sensors.h"
#include <Arduino.h>
#include <SD.h>

File file;
const byte CS_PIN = 29;
const char* FILE_NAME = "sensors_data.csv";

void gps_sd_init() {
  if (!SD.begin(CS_PIN)) {
    Serial.println("SD init failed");
  }
  file = SD.open(FILE_NAME, FILE_WRITE);
  if (!file) {
    Serial.println("Error opening file");
  }
  else {
    if (file.size() == 0) {
      file.println("pressure,humidity,temperature,distance");
    }
    file.flush();
  }
}

void gps_sd_update(double sensors_data[4]) {
    static unsigned long last_save = 0;
      file.print(sensors_data[0]);
      file.print(",");
      file.print(sensors_data[1]);
      file.print(",");
      file.print(sensors_data[2]);
      file.print(",");
      file.println(sensors_data[3]);
      file.flush();
}

void gps_sd_save_now(double sensors_data[3]) {
 
    if (file){
    float dist = 0.0;

    file.print(sensors_data[0]);
    file.print(",");
    file.print(sensors_data[1]);
    file.print(",");
    file.print(sensors_data[2]);
    file.print(",");
    file.println(dist);
    file.flush();
    Serial.println("Manual save triggered");
    }
}
