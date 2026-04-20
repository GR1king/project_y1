#ifndef __VIND_H
#define __VIND_H
 
#include "gd32vf103.h"
#include <stdint.h>
#include <math.h>
 
/* Flagga som sätts av timer-avbrottet */
extern volatile uint8_t read_flag;
 
// Konfigurerar timer för vindsensorn //
void timer_config(uint32_t timer_periph, uint32_t time_interval_ms);
 
/* Läser från ett I2C-register */
void i2c_read_register(int32_t port, uint8_t addr, uint8_t reg, uint8_t size, uint8_t *data);
 
/* Skriver till ett I2C-register */
void i2c_write_register(int32_t port, uint8_t addr, uint8_t reg, uint8_t size, uint8_t *data);
 
/* Beräknar medelvinkeln från en array av vinklar */
float getAverageAngle(float angles[], int count);
 
#endif
