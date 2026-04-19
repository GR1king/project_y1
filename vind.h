//Separat fil där vindsensorn kopplas till temp sensor koden
#ifndef __VIND_H
#define __VIND_H
 
#include "gd32vf103.h"
#include <stdint.h>
#include <math.h>

/* AS5600 vindsensor I2C-adress */
#define WIND_SENSOR_ADDR 0x36
 
/* Läser vinkeln från sensorn och returnerar grader (0.0–360.0) */
float wind_read_degrees(void);
 
/* Beräknar medelvinkeln från en array av vinklar */
float wind_get_average(float angles[], int count);
 
#endif
