#include <SoftwareSerial.h>

#include "wheels.h"
#include "sensors.h"
#include "gps_sd.h"
#include "bluetooth.h"
#include "wifi_control.h"

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
  wifi_init();
}

void loop() {
  static double sensors_data[3];

  double gps_lat = 0.0, gps_lon = 0.0;
  float gps_speed = 0.0, gps_alt = 0.0;

  sensors_read(sensors_data);

  if (gps_get_valid_data(&gps_lat, &gps_lon, &gps_speed, &gps_alt)) {
    wifi_send_data(sensors_data, gps_lat, gps_lon, gps_speed, gps_alt);

    double dest_lat, dest_lon;
    if (wifi_has_destination()) {
      wifi_request_destination(&dest_lat, &dest_lon);
      Serial.print("Navigate to: ");
      Serial.print(dest_lat, 6);
      Serial.print(", ");
      Serial.println(dest_lon, 6);
    }
  }

  // Проверяем, в каком режиме работаем
  if (digitalRead(STATE_PIN)) {
    // --- РЕЖИМ Wi-Fi ---
    digitalWrite(LED_BUILTIN, HIGH); // Индикатор: Wi-Fi активен

    // Обрабатываем команды, пришедшие от Wi-Fi модуля (ESP8266)
    wifi_handle_incoming_commands();

  } else {
    // --- РЕЖИМ Bluetooth ---
    digitalWrite(LED_BUILTIN, LOW); // Индикатор: Bluetooth активен

    // Обрабатываем команды, пришедшие по Bluetooth
    if (bluetooth.available()) {
      char cmd = bluetooth.read();
      // Передаём команду в модуль управления моторами
      handle_motor_command(cmd); // Эта функция из motors.cpp
    }
  }
  gps_sd_update(sensors_data);
}
