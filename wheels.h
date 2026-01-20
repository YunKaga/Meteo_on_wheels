#ifndef WHEELS_H
#define WHEELS_H

void wheels_init(byte ena);
void w_off();
void move_b();
void move_f();
void rotation_l();
void rotation_r();
void move_r();
void move_l();
void move_fr();
void move_fl();
void move_br();
void move_bl();
void handle_wheel_command(char data);
void speed_change(int value);

#endif
