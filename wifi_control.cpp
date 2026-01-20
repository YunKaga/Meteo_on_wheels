#include "wifi_control.h"
#include <SoftwareSerial.h>
#include <Arduino.h>
#include "sensors.h"
#include "wheels.h"

//SoftwareSerial wifiSerial(1, 0);
#define wifiSerial    Serial1


static bool save_requested = false;

void send_at_command(const char* cmd, unsigned long timeout) {
    wifiSerial.write(cmd);
    Serial.print("AT: ");
    Serial.println(cmd);
    delay(100);
}

bool wait_for_response(const char* expected, unsigned long timeout) {
    unsigned long start = millis();
    String response = "";
    while (millis() - start < timeout) {
        while (wifiSerial.available()) {
            char c = wifiSerial.read();
            response += c;
            Serial.print(c);
            if (response.indexOf(expected) >= 0) {
                return true;
            }
        }
        delay(10);
    }
    return false;
}

void wifi_init() {
    wifiSerial.begin(115200);
    delay(1000);
    send_at_command("AT\r\n", 2000);
    if (!wait_for_response("OK")) {
        Serial.println("ESP8266 not responding to AT command");
        return;
    }
    send_at_command("AT+CWMODE=1\r\n", 2000);
    if (!wait_for_response("OK")) {
        Serial.println("Failed to set station mode");
        return;
    }
    send_at_command("AT+CWJAP=\"KATANA_17 4207\",\"35u979B&\"\r\n", 9000);
    if (!wait_for_response("OK")) {
        Serial.println("Failed to connect to WiFi");
        return;
    }
    // Запуск TCP-сервера
    send_at_command("AT+CIPMUX=1\r\n", 2000);
    send_at_command("AT+CIPSERVER=1,8080\r\n", 3000);
    Serial.println("WiFi connected via ESP8266");
}

void wifi_handle_incoming_commands() {
  static String buffer = "";
  while (wifiSerial.available()) {
    char c = wifiSerial.read();
    Serial.write(c); // эхо для отладки
    buffer += c;

    if (c == '\n') {
      // Ищем начало данных после +IPD,...
      int colonIndex = buffer.indexOf(':');
      if (colonIndex != -1 && buffer.startsWith("+IPD,")) {
        String dataPart = buffer.substring(colonIndex + 1);
        dataPart.trim(); // убираем \r\n

        if (dataPart.startsWith("CMD:")) {
          char cmd = dataPart.charAt(4);
          Serial.println("→ Command: " + String(cmd));
          handle_wheel_command(cmd);
        } else if (dataPart == "SAVE_NOW") {
          save_requested = true;
          Serial.println("→ Save requested");
        }
      }
      buffer = "";
    }

    // Защита от переполнения
    if (buffer.length() > 200) buffer = "";
  }
}

void wifi_send_data(double sensors_data[3]) {
    //float dist = read_ultrasonic_cm();
    String data = "DATA:";
    data += String(sensors_data[0], 2);
    data += ",";
    data += String(sensors_data[1], 2);
    data += ",";
    data += String(sensors_data[2], 2);
    data += ",";
    data += String(sensors_data[3], 0);
    data += "\n";

    String cmd = "AT+CIPSEND=0," + String(data.length()) + "\r\n";
    send_at_command(cmd.c_str(), 1000);
    delay(100);
    wifiSerial.print(data);
    delay(100);
}

bool wifi_should_save_now(){
    return save_requested;
}

void wifi_clear_save_flag(){
    save_requested = false;
}
