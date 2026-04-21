/* ========================= sht31.h ========================= */

#ifndef __SHT31_H
#define __SHT31_H

#include "gd32vf103.h"
#include <stdint.h>

/* temperature limits used in main.c */
#define temp_high 28
#define temp_low  18

/* functions that main.c can use */
void sht31_init_setup(void);
int sht31_read(uint16_t *temp_raw, uint16_t *hum_raw);
void sht31_convert(uint16_t temp_raw, uint16_t hum_raw, int *temp_c, int *hum_pct);
float sht31_wet_bulb(float temp_c, float hum_pct);

#endif
