#include <Arduino.h>
#include "wheels.h"

const byte BR0 = 2;
const byte BR1 = 3;
const byte BL0 = 4;
const byte BL1 = 5;
const byte FL0 = 9;
const byte FL1 = 8;
const byte FR0 = 10;
const byte FR1 = 11;

static byte ena_pin;

void wheels_init(byte ena){
  ena_pin = ena;

  pinMode(ena_pin, OUTPUT);
  pinMode(BR0, OUTPUT);
  pinMode(BR1, OUTPUT);
  pinMode(BL0, OUTPUT);
  pinMode(BL1, OUTPUT);
  pinMode(FL0, OUTPUT);
  pinMode(FL1, OUTPUT);
  pinMode(FR0, OUTPUT);
  pinMode(FR1, OUTPUT);
  analogWrite(ena_pin, 150);
}

void w_off(){
  digitalWrite(BR0, LOW);
  digitalWrite(BR1, LOW);
  digitalWrite(BL0, LOW);
  digitalWrite(BL1, LOW);
  digitalWrite(FL0, LOW);
  digitalWrite(FL1, LOW);
  digitalWrite(FR0, LOW);
  digitalWrite(FR1, LOW);
}

// Motion functions
void move_b(){
  digitalWrite(BR0, HIGH);
  digitalWrite(BR1, LOW);
  digitalWrite(BL0, HIGH);
  digitalWrite(BL1, LOW);
  digitalWrite(FL0, HIGH);
  digitalWrite(FL1, LOW);
  digitalWrite(FR0, HIGH);
  digitalWrite(FR1, LOW);
}

void move_f(){
  digitalWrite(BR0, LOW);
  digitalWrite(BR1, HIGH);
  digitalWrite(BL0, LOW);
  digitalWrite(BL1, HIGH);
  digitalWrite(FL0, LOW);
  digitalWrite(FL1, HIGH);
  digitalWrite(FR0, LOW);
  digitalWrite(FR1, HIGH);
}

void rotation_l(){
  digitalWrite(BR0, HIGH);
  digitalWrite(BR1, LOW);
  digitalWrite(BL0, LOW);
  digitalWrite(BL1, HIGH);
  digitalWrite(FL0, LOW);
  digitalWrite(FL1, HIGH);
  digitalWrite(FR0, HIGH);
  digitalWrite(FR1, LOW);
}

void rotation_r(){
  digitalWrite(BR0, LOW);
  digitalWrite(BR1, HIGH);
  digitalWrite(BL0, HIGH);
  digitalWrite(BL1, LOW);
  digitalWrite(FL0, HIGH);
  digitalWrite(FL1, LOW);
  digitalWrite(FR0, LOW);
  digitalWrite(FR1, HIGH);
}

void move_r(){
  digitalWrite(BR0, LOW);
  digitalWrite(BR1, HIGH);
  digitalWrite(BL0, HIGH);
  digitalWrite(BL1, LOW);
  digitalWrite(FL0, LOW);
  digitalWrite(FL1, HIGH);
  digitalWrite(FR0, HIGH);
  digitalWrite(FR1, LOW);
}

void move_l(){
  digitalWrite(BR0, HIGH);
  digitalWrite(BR1, LOW);
  digitalWrite(BL0, LOW);
  digitalWrite(BL1, HIGH);
  digitalWrite(FL0, HIGH);
  digitalWrite(FL1, LOW);
  digitalWrite(FR0, LOW);
  digitalWrite(FR1, HIGH);
}

void move_fr(){
  digitalWrite(BR0, LOW);
  digitalWrite(BR1, HIGH);
  digitalWrite(BL0, LOW);
  digitalWrite(BL1, LOW);
  digitalWrite(FL0, LOW);
  digitalWrite(FL1, HIGH);
  digitalWrite(FR0, LOW);
  digitalWrite(FR1, LOW);
}

void move_fl(){
  digitalWrite(BR0, LOW);
  digitalWrite(BR1, LOW);
  digitalWrite(BL0, LOW);
  digitalWrite(BL1, HIGH);
  digitalWrite(FL0, LOW);
  digitalWrite(FL1, LOW);
  digitalWrite(FR0, LOW);
  digitalWrite(FR1, HIGH);
}

void move_br(){
  digitalWrite(BR0, LOW);
  digitalWrite(BR1, LOW);
  digitalWrite(BL0, HIGH);
  digitalWrite(BL1, LOW);
  digitalWrite(FL0, LOW);
  digitalWrite(FL1, LOW);
  digitalWrite(FR0, HIGH);
  digitalWrite(FR1, LOW);
}

void move_bl(){
  digitalWrite(BR0, HIGH);
  digitalWrite(BR1, LOW);
  digitalWrite(BL0, LOW);
  digitalWrite(BL1, LOW);
  digitalWrite(FL0, HIGH);
  digitalWrite(FL1, LOW);
  digitalWrite(FR0, LOW);
  digitalWrite(FR1, LOW);
}

void handle_wheel_command(char data){
    Serial.print("Handling command: '");
    Serial.print(data);
    Serial.println("'");
  if (data == '0'){w_off();}
  else if (data == 'F'){move_f();}
  else if (data == 'B'){move_b();}
  else if (data == '6'){rotation_r();}
  else if (data == '5'){rotation_l();}
  else if (data == 'R'){move_r();}
  else if (data == 'L'){move_l();}
  else if (data == '3'){move_fr();}
  else if (data == '1'){move_fl();}
  else if (data == '4'){move_br();}
  else if (data == '2'){move_bl();}
}
