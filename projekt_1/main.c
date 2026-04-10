#include <stdio.h>
#include "gd32vf103.h"
#include "sht31.c" //Vi använder SHT35
#include "usart.h"

#define SHT31_ADDR 

static int system_aktiv = 1;

void check_temp(int temp_c) {
    if (temp_c > temp_high) {
        putstr("!! VARNING: Temp for HOG !!\r\n");
    } 
    else if (temp_c < temp_low) {
        putstr("!! VARNING: Temp for LAG !!\r\n");
    }
    else {
        putstr(">> Temp normal\r\n");
    }
}



int main (void) {
    usart_init();
    i2c_init_setup();   


}


//Temperatur och luftfuktighetsensor