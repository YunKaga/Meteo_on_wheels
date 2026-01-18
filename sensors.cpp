#include "sensors.h"
#include <Arduino.h>
#include <GyverBME280.h>
#include <DHT22.h>
#include <Wire.h>
#include <NewPing.h>

GyverBME280 bmp;
DHT22 dht22(31);

#define TRIG_PIN 22
#define ECHO_PIN 23
#define MAX_DISTANCE 200
NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);

void sensors_init(){
  Wire.begin();
  bmp.begin();
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

void sensors_read(double data[]){
  data[0] = bmp.readPressure();
  data[1] = dht22.getHumidity();
  data[2] = dht22.getTemperature();
}

float read_ultrasonic_cm() {
  return sonar.ping_cm();
}
