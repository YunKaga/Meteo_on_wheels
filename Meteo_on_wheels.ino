#include <SoftwareSerial.h>

#include "wheels.h"
#include "sensors.h"
#include "gps_sd.h"
#include "bluetooth.h"

// Bluetooth settings
SoftwareSerial bluetooth(0, 1); // RX, TX
byte State = 7;
char data = '0';

// Speed
byte Ena = 6;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT); // LED

  Serial.begin(9600);
  bluetooth.begin(38400);
  pinMode(State, INPUT);

  wheels_init(Ena);
  sensors_init();
  gps_sd_init();
}

void loop() {
  static double sensors_data[3];
  sensors_read(sensors_data);

  if (digitalRead(State)){
    digitalWrite(LED_BUILTIN, HIGH); // Turn on LED on Arduino
    char data = bluetooth.read();
    handle_wheel_command(data);
  }
  
  else {
    digitalWrite(LED_BUILTIN, LOW); // Turn off LED on Arduino
    w_off();
  }

  gps_sd_update(sensors_data);
}
