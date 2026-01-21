#include "sd.h"
#include <Arduino.h>
#include <SD.h>
#include "wheels.h"

const byte CS_PIN = 29;
const char* ROUTE_FILE = "route.log";

void sd_init() {
    if (!SD.begin(CS_PIN)) {
        Serial.println("SD init failed");
    }
}

void sd_log_motion(char cmd, unsigned long duration_ms) {
    File f = SD.open(ROUTE_FILE, FILE_WRITE);
    if (f) {
        f.print(cmd);
        f.print(',');
        f.println(duration_ms);
        f.close();
        Serial.print("Logged route: ");
        Serial.print(cmd);
        Serial.print(" for ");
        Serial.print(duration_ms);
        Serial.println(" ms");
    } else {
        Serial.println("Failed to open route.log");
    }
}

char invert_command(char cmd) {
    switch (cmd) {
        case 'F': return 'B';
        case 'B': return 'F';
        case 'L': return 'R';
        case 'R': return 'L';
        case '1': return '4'; // FL → BR
        case '2': return '3'; // BL → FR
        case '3': return '2'; // FR → BL
        case '4': return '1'; // BR → FL
        case '5': return '6'; // rot L → rot R
        case '6': return '5'; // rot R → rot L
        default: return '0';
    }
}

void sd_replay_route_reverse() {
    File f = SD.open(ROUTE_FILE, FILE_READ);
    if (!f) {
        Serial.println("No route to replay");
        return;
    }

    const int MAX_STEPS = 200;
    struct Step { char cmd; unsigned long dur; };
    Step steps[MAX_STEPS];
    int count = 0;

    while (f.available() && count < MAX_STEPS) {
        String line = f.readStringUntil('\n');
        int comma = line.indexOf(',');
        if (comma > 0) {
            char cmd = line.charAt(0);
            unsigned long dur = line.substring(comma + 1).toInt();
            steps[count++] = {cmd, dur};
        }
    }
    f.close();

    Serial.println("Replaying route in reverse...");
    for (int i = count - 1; i >= 0; i--) {
        char inv = invert_command(steps[i].cmd);
        if (inv != '0') {
            handle_wheel_command(inv);
            delay(steps[i].dur); // можно заменить на более точный таймер при необходимости
        }
    }
    w_off(); // остановка в конце
    SD.remove(ROUTE_FILE);
    Serial.println("Route replay finished and log cleared.");
}

bool sd_has_route() {
    return SD.exists(ROUTE_FILE);
}
