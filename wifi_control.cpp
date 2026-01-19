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
    send_at_command("AT+CWJAP=\"Archer\",\"12141250\"\r\n", 9000);
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
  static String inputBuffer = "";
  while (wifiSerial.available()) {
    char c = wifiSerial.read();
    Serial.print(c); 
    if (c == '\n') {
      inputBuffer.trim();
      if (inputBuffer.startsWith("CMD:")) {
        char motor_cmd = inputBuffer.charAt(4);
        Serial.print("→ Parsed motor command: ");
        Serial.println(motor_cmd);
        handle_wheel_command(motor_cmd);
      } else if (inputBuffer == "SAVE_NOW") {
        save_requested = true;
        Serial.println("→ Save requested via WiFi");
      }
      inputBuffer = ""; 
    } else if (c != '\r') {
      inputBuffer += c;
    }
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
    //data += ",";
    //data += String(dist, 1);
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
