#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include "wheels.h"

inline void handle_bluetooth_command(char cmd) {
  handle_wheel_command(cmd);
}

#endif
