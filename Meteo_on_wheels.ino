#include <SoftwareSerial.h>

#include "wheels.h"
#include "sensors.h"
#include "sd.h"
#include "wifi_control.h"

unsigned long last_command_time = 0;

const byte STATE_PIN = 7;
char data = '0';

// Speed
byte Ena = 6;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(9600);
  pinMode(STATE_PIN, INPUT);

  wheels_init(Ena);
  sensors_init();
  sd_init();
  wifi_init();
}

void loop() {
  static double sensors_data[4];
  sensors_read(sensors_data);
  wifi_send_data(sensors_data);

  wifi_handle_incoming_commands();
  if (millis() - last_command_time > 5000 && sd_has_route()) {
    Serial.println("Connection lost! Returning home...");
    digitalWrite(LED_BUILTIN, HIGH);
    sd_replay_route_reverse();
    digitalWrite(LED_BUILTIN, LOW);
    last_command_time = millis();
  }
  delay(100);
}
