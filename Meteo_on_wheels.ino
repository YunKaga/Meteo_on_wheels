#include <SoftwareSerial.h>

#include "wheels.h"
#include "sensors.h"
#include "sd.h"
#include "bluetooth.h"
#include "wifi_control.h"

const byte STATE_PIN = 7;
char data = '0';

// Speed
byte Ena = 6;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT); // LED

  Serial.begin(9600);
  pinMode(STATE_PIN, INPUT);

  wheels_init(Ena);
  sensors_init();
  //gps_sd_init();
  wifi_init();
}

void loop() {
  static double sensors_data[4];

  sensors_read(sensors_data);
  wifi_send_data(sensors_data);

  //if (Serial.available()) Serial1.write(Serial.read());
  //if (Serial1.available()) Serial.write(Serial1.read());
  
  wifi_handle_incoming_commands();
  //gps_sd_update(sensors_data);
  delay(100);
}
