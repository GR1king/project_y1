#ifndef __sht31_h
#define __sht31_h
//VI ANVÄNDER SHT31
#include "gd32vf103.h"

#define temp_high 28 //ändra senare
#define temp_low 18 

void i2c_init_setup(void);
int  sht31_read(int16_t *temp_raw, uint16_t *hum_raw);
void sht31_convert(int16_t temp_raw, uint16_t hum_raw,
int *temp_c, int *hum_pct);

#endif