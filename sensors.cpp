#include "sensors.h"
#include <Arduino.h>
#include <GyverBME280.h>
#include <DHT22.h>
#include <Wire.h>

GyverBME280 bmp;
DHT22 dht22(31);

void sensors_init(){
  Wire.begin();
  bmp.begin();
}

void sensors_read(double data[]){
  data[0] = bmp.readPressure();
  data[1] = dht22.getHumidity();
  data[2] = dht22.getTemperature();
}
