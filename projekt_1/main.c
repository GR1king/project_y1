#include "gd32vf103.h"   /* MCU library */
#include "sht31.h"       /* our sensor functions */
#include "vind.h"        /* wind sensor functions */
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
    int ret;             /* return value from sensor function */
    int temp_abs;        /* used if temperature is negative */
    float wet_bulb;      /* calculated wet bulb temperature */

    /* wind sensor variables */
    float angles[10];
    int wind_index    = 0;
    uint16_t wind_avg = 0;
    int average_flag  = 0;

    /* start timer avbrott för vindsensorn */
    eclic_global_interrupt_enable();
    eclic_set_nlbits(ECLIC_GROUP_LEVEL3_PRIO1);
    eclic_irq_enable(TIMER5_IRQn, 1, 0);
    timer_config(TIMER5, 500);

    t5omsi();   /* start Timer5 as a 1 ms timer */

    /* start LCD */
    Lcd_SetType(LCD_INVERTED);
    Lcd_Init();
    LCD_Clear(BLACK);

    /* start I2C — används av både SHT31 och vindsensorn */
    i2c_init_setup();

    while (1) {
        /* --- Vindsensor: läs vinkel via timer-avbrott --- */
        if (read_flag) {
            read_flag = 0;
            uint8_t buf[2];
            i2c_read_register(I2C0, 0x36, 0x0C, 2, buf);
            uint16_t angle = (buf[0] << 8) | buf[1];
            float degree = (angle * 360.0f) / 4095.0f;
            angles[wind_index] = degree;
            wind_index++;

            if (wind_index >= 10) {
                wind_avg     = (uint16_t)getAverageAngle(angles, 10);
                average_flag = 1;
                wind_index   = 0;
            }
        }

        /* --- SHT31: läs temperatur och luftfuktighet --- */
        ret = sht31_read(&temp_raw, &hum_raw);

        /* clear the screen before drawing new text */
        LCD_Clear(BLACK);

        /* show fixed labels */
        LCD_ShowStr(0, 0,  (u8 *)"SHT31 Sensor", YELLOW, OPAQUE);
        LCD_ShowStr(0, 15, (u8 *)"Temp:",        WHITE,  OPAQUE);
        LCD_ShowStr(0, 30, (u8 *)"Humidity:",    WHITE,  OPAQUE);
        LCD_ShowStr(0, 45, (u8 *)"Wet bulb:",    WHITE,  OPAQUE);
        LCD_ShowStr(0, 60, (u8 *)"Wind angle:",  WHITE,  OPAQUE);

        if (ret == 0) {
            /* change raw values into real values */
            sht31_convert(temp_raw, hum_raw, &temp_c, &hum_pct);
            wet_bulb = sht31_wet_bulb((float)temp_c, (float)hum_pct);

            /* show temperature */
            if (temp_c < 0) {
                temp_abs = -temp_c;
                LCD_ShowChar(45, 15, '-', OPAQUE, CYAN);
                LCD_ShowNum(53, 15, (u16)temp_abs, 3, CYAN);
            } else {
                LCD_ShowNum(45, 15, (u16)temp_c, 3, CYAN);
            }
            LCD_ShowChar(80, 15, 'C', OPAQUE, CYAN);

            /* show humidity */
            LCD_ShowNum(75, 30, (u16)hum_pct, 3, CYAN);
            LCD_ShowChar(102, 30, '%', OPAQUE, CYAN);

            /* show wet bulb */
            LCD_ShowNum1(75, 45, wet_bulb, 4, GREEN);
            LCD_ShowChar(115, 45, 'C', OPAQUE, GREEN);

            /* show temperature status */
            if (temp_c > temp_high) {
                LCD_ShowStr(0, 75, (u8 *)"Temp too high", RED,   OPAQUE);
            } else if (temp_c < temp_low) {
                LCD_ShowStr(0, 75, (u8 *)"Temp too low",  BLUE,  OPAQUE);
            } else {
                LCD_ShowStr(0, 75, (u8 *)"Temp normal",   GREEN, OPAQUE);
            }
        } else {
            LCD_ShowStr(0, 75, (u8 *)"Error:", RED, OPAQUE);
        }

        /* show wind angle when ready */
        if (average_flag) {
            average_flag = 0;
            LCD_ShowNum(75, 60, wind_avg, 3, CYAN);
            LCD_ShowChar(102, 60, 'd', OPAQUE, CYAN);  /* d for degrees */
        }

        LCD_Wait_On_Queue();   /* make sure LCD finishes drawing */
        delay_ms(1000);        /* wait 1 second */
    }
}
