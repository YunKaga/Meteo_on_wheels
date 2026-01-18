#ifndef WIFI_CONTROL_H
#define WIFI_CONTROL_H

void wifi_init();
void wifi_handle_incoming_commands();
void wifi_send_data(double sensors_data[3]);

void send_at_command(const char* cmd, unsigned long timeout = 5000);
bool wait_for_response(const char* expected, unsigned long timeout = 5000);

bool wifi_should_save_now();
void wifi_clear_save_flag();
#endif
