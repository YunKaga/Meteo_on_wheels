#ifndef SD_H
#define SD_H

void sd_init();
void sd_log_motion(char cmd, unsigned long duration_ms);
void sd_replay_route_reverse();
bool sd_has_route();

#endif
