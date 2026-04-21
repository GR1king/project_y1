#ifndef __VIND_H
#define __VIND_H

#include <stdint.h>

#define VIND_SENSOR_ADDR 0x36

void vind_init_setup(void);
int vind_read_raw(uint16_t *vind_raw);
void vind_read_degrees(uint16_t vind_raw, int *vind_deg);
float getAverageAngle(float angles[], int count);
#endif
