#include "gd32vf103.h"   /* MCU library */
#include "sht31.h"       /* our sensor functions */
#include "drivers.h"     /* timer functions */
#include "lcd.h"         /* LCD functions */

/* Wait for a number of milliseconds */
static void delay_ms(int ms) {
    while (ms > 0) {
        while (!t5expq()) {
            /* stay here until 1 ms has passed */
        }
        ms--;
    }
}

int main(void) {
    uint16_t temp_raw;   /* raw temperature from sensor */
    uint16_t hum_raw;    /* raw humidity from sensor */
    int temp_c;          /* temperature in Celsius */
    int hum_pct;         /* humidity in percent */
    int ret_sht35;             /* return value from sensor function */
    int temp_abs;        /* used if temperature is negative */
    float wet_bulb;      /* calculated wet bulb temperature */
    t5omsi();   /* start Timer5 as a 1 ms timer */

    /* start LCD */
    Lcd_SetType(LCD_INVERTED);
    Lcd_Init();
    LCD_Clear(BLACK);

    /* visa direkt att programmet lever */
    LCD_ShowStr(0, 0, (u8 *)"Starting...", YELLOW, OPAQUE);
    LCD_Wait_On_Queue();
    delay_ms(500);

    /* start I2C */
    sht31_init_setup();
    
        /* main loop */
    while (1) {
        /* ask the sensor for one reading */
        ret_sht35 = sht31_read(&temp_raw, &hum_raw);
        /* clear the screen before drawing new text */
        LCD_Clear(BLACK);
        /* show fixed labels */
        LCD_ShowStr(0, 0, (u8 *)"Temp:", WHITE, OPAQUE);
        LCD_ShowStr(0, 15, (u8 *)"Humidity:", WHITE, OPAQUE);
        LCD_ShowStr(0, 30, (u8 *)"Wet bulb:", WHITE, OPAQUE);
        

        if (ret_sht35 == 0) {
            /* change raw values into real values */
            sht31_convert(temp_raw, hum_raw, &temp_c, &hum_pct);
            wet_bulb = sht31_wet_bulb((float)temp_c, (float)hum_pct);

            /* show temperature */
            if (temp_c < 0) {
                temp_abs = -temp_c;                            /* make it positive */
                LCD_ShowChar(45, 0, '-', OPAQUE, CYAN);      /* show minus sign */
                LCD_ShowNum(53, 0, (u16)temp_abs, 3, CYAN);  /* show number */
            } else {
                LCD_ShowNum(45, 0, (u16)temp_c, 3, CYAN);    /* show positive number */
            }
            LCD_ShowChar(80, 0, 'C', OPAQUE, CYAN);          /* show C */

            /* show humidity */
            LCD_ShowNum(75, 15, (u16)hum_pct, 3, CYAN);       /* show humidity number */
            LCD_ShowChar(102, 15, '%', OPAQUE, CYAN);  
            
            /* Wet bulb */
            LCD_ShowNum1(75, 30, wet_bulb, 4, GREEN);
            LCD_ShowChar(115, 30, 'C', OPAQUE, GREEN);/* show % */

            /* show if temperature is high, low, or normal */
            if (temp_c > temp_high) {
                LCD_ShowStr(0, 45, (u8 *)"Temp too high", RED, OPAQUE);
            } else if (temp_c < temp_low) {
                LCD_ShowStr(0, 45, (u8 *)"Temp too low", BLUE, OPAQUE);
            } else {
                LCD_ShowStr(0, 45, (u8 *)"Temp normal", GREEN, OPAQUE);
            }
        } else {
            /* if reading failed, show error */
            LCD_ShowStr(0, 45, (u8 *)"Error:", RED, OPAQUE);
            
        }

        LCD_Wait_On_Queue();   /* make sure LCD finishes drawing */
        delay_ms(1000);        /* wait 1 second */
    }
}