#ifndef WIFI_CONTROL_H
#define WIFI_CONTROL_H

#include <Arduino.h>

// Используем SoftwareSerial для связи с ESP8266
extern SoftwareSerial wifiSerial;

void wifi_init();
void wifi_handle_incoming_commands();
void wifi_send_data(double sensors_data[3], double gps_lat, double gps_lon, float gps_speed, float gps_alt);
void wifi_request_destination(double* lat, double* lon);
bool wifi_has_destination();

// Вспомогательные функции для AT-команд
void send_at_command(const char* cmd, unsigned long timeout = 5000);
bool wait_for_response(const char* expected, unsigned long timeout = 5000);

#endif
