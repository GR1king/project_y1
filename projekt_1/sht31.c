#ifndef __SHT31_H
#define __SHT31_H

#include "gd32vf103.h"
#include <stdint.h>


#define temp_high 28
#define temp_low  18


void i2c_init_setup(void);
int sht31_read(uint16_t *temp_raw, uint16_t *hum_raw);
void sht31_convert(uint16_t temp_raw, uint16_t hum_raw, int *temp_c, int *hum_pct);
float sht31_wet_bulb(float temp_c, float hum_pct);

#endif




