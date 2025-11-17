#include "wifi_control.h"
#include <SoftwareSerial.h>
#include <Arduino.h>

SoftwareSerial wifiSerial(15, 14); // RX, TX

static bool destination_set = false;
static double dest_lat = 0.0, dest_lon = 0.0;

// --- Вспомогательные функции для AT-команд ---
void send_at_command(const char* cmd, unsigned long timeout) {
  wifiSerial.println(cmd);
  delay(100); // дадим время на обработку
}

bool wait_for_response(const char* expected, unsigned long timeout) {
  unsigned long start = millis();
  String response = "";
  while (millis() - start < timeout) {
    if (wifiSerial.available()) {
      char c = wifiSerial.read();
      response += c;
      if (response.indexOf(expected) >= 0) {
        return true;
      }
    }
    delay(10);
  }
  return false;
}

// --- Инициализация ---
void wifi_init() {
  wifiSerial.begin(115200);
  delay(1000);

  send_at_command("AT", 2000);
  if (!wait_for_response("OK")) {
    Serial.println("ESP8266 not responding to AT command");
    return;
  }

  // Установка режима клиента
  send_at_command("AT+CWMODE=1", 2000);
  if (!wait_for_response("OK")) {
    Serial.println("Failed to set station mode");
    return;
  }

  // Подключение к Wi-Fi
  send_at_command("AT+CWJAP=\"YunKage\",\"2()()618()7\"", 10000);
  if (!wait_for_response("OK")) {
    Serial.println("Failed to connect to WiFi");
    return;
  }

  Serial.println("WiFi connected via ESP8266");
}

// --- Обработка команд от Wi-Fi ---
void wifi_handle_incoming_commands() {
  if (wifiSerial.available()) {
    String cmd = wifiSerial.readStringUntil('\n');
    cmd.trim();

    if (cmd.startsWith("CMD:")) {
      char motor_cmd = cmd.charAt(4);
      Serial.print("Received motor command via WiFi: ");
      Serial.println(motor_cmd);
      // Передаём команду в модуль управления моторами
      // handle_motor_command(motor_cmd); // Вызываем функцию из motors.cpp
    }
    else if (cmd.startsWith("SET_DEST:")) {
      int sep = cmd.indexOf(',');
      if (sep > 0) {
        dest_lat = cmd.substring(9, sep).toDouble();
        dest_lon = cmd.substring(sep + 1).toDouble();
        destination_set = true;
        Serial.println("Destination set via WiFi.");
      }
    }
  }
}

// --- Отправка данных ---
void wifi_send_data(double sensors_data[3], double gps_lat, double gps_lon, float gps_speed, float gps_alt) {
  String data = "DATA:";
  data += gps_lat, 6;
  data += ",";
  data += gps_lon, 6;
  data += ",";
  data += gps_speed;
  data += ",";
  data += gps_alt;
  data += ",";
  data += sensors_data[0];
  data += ",";
  data += sensors_data[1];
  data += ",";
  data += sensors_data[2];
  data += "\n";
  wifiSerial.print(data);
}

bool wifi_has_destination() {
  return destination_set;
}

void wifi_request_destination(double* lat, double* lon) {
  if (destination_set) {
    *lat = dest_lat;
    *lon = dest_lon;
    destination_set = false;
  }
}
