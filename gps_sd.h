#ifndef GPS_SD_H
#define GPS_SD_H
void gps_sd_init();
void gps_sd_update(double sensors_data[]);
void sd_save_now(double sensord_data[3]);
#endif
