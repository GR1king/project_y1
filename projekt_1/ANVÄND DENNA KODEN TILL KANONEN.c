#include "gd32vf103.h"   /* MCU library */
#include "sht31.h"       /* our sensor functions */
#include "drivers.h"     /* timer functions */
#include "lcd.h"         /* LCD functions */
#include "vind.h"        /* vind sensor functions */
#include "led.h"         /* led functions */
#include "pwm.h"
#include "math.h"

#define EXTRA 10
#define SPEED 5

void rotera(int pinMal, int nuvarandeLage);

/* Wait for a number of milliseconds */
static void delay_ms(int ms);

int main(void) {
    uint16_t temp_raw;
    uint16_t hum_raw;
    int temp_c;
    int hum_pct;
    int ret_sht35;
    int temp_abs;
    float wet_bulb;

    uint16_t vind_raw;
    uint16_t new_average = 0;
    int vind_deg;
    int ret_vind;
    float angles[10];
    int index = 0;
    int average_flag = 0;

    int onskatLage;
    uint32_t lage;
    int toggle = 0;
    int grader = 0;
    int bGrader = -1;

    int nuvarandeLage = 1;

    t5omsi();

    /* start LCD */
    Lcd_SetType(LCD_INVERTED);
    Lcd_Init();
    LCD_Clear(BLACK);

    LCD_ShowStr(0, 0, (u8 *)"Starting...", YELLOW, OPAQUE);
    LCD_Wait_On_Queue();
    delay_ms(500);

    /* start I2C */
    sht31_init_setup();
    vind_init_setup(); // Only initialize once at startup
    led_init();

    /* Initsera Motor */
    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_GPIOA);

    gpio_init(GPIOB, GPIO_MODE_IPD, GPIO_OSPEED_50MHZ, GPIO_PIN_11); // 0 och 360 grader
    gpio_init(GPIOA, GPIO_MODE_IPD, GPIO_OSPEED_50MHZ, GPIO_PIN_5);  // 315 grader
    gpio_init(GPIOB, GPIO_MODE_IPD, GPIO_OSPEED_50MHZ, GPIO_PIN_8);  // 270 grader
    gpio_init(GPIOB, GPIO_MODE_IPD, GPIO_OSPEED_50MHZ, GPIO_PIN_5);  // 180 grader
    gpio_init(GPIOB, GPIO_MODE_IPD, GPIO_OSPEED_50MHZ, GPIO_PIN_4);  // 135 grader
    gpio_init(GPIOB, GPIO_MODE_IPD, GPIO_OSPEED_50MHZ, GPIO_PIN_12); // 45 grader
    gpio_init(GPIOA, GPIO_MODE_IPD, GPIO_OSPEED_50MHZ, GPIO_PIN_8);  // 90 grader
    gpio_init(GPIOA, GPIO_MODE_IPD, GPIO_OSPEED_50MHZ, GPIO_PIN_7);  // 225 grader


    T1powerUpInitPWM(0x0C);



    nuvarandeLage = 1;
   


    while (1) {

    LCD_Clear(BLACK);
    LCD_ShowStr(0, 0, (u8 *)"Loop start", GREEN, OPAQUE);
    LCD_Wait_On_Queue();
        ret_sht35 = sht31_read(&temp_raw, &hum_raw);

        LCD_ShowStr(0, 15, (u8 *)"VINDSENSORN HAR STANNAT", GREEN, OPAQUE);
        LCD_Wait_On_Queue();

        ret_vind = vind_read_raw(&vind_raw);
      
        LCD_ShowStr(0, 30, (u8 *)"After wind", GREEN, OPAQUE);
        LCD_Wait_On_Queue();


  

        LCD_Clear(BLACK);

        LCD_ShowStr(0, 0, (u8 *)"Temp:", WHITE, OPAQUE);
        LCD_ShowStr(0, 15, (u8 *)"Humidity:", WHITE, OPAQUE);
        LCD_ShowStr(0, 30, (u8 *)"Wet bulb:", WHITE, OPAQUE);
        LCD_ShowStr(0, 45, (u8 *)"Wind Direction:", WHITE, OPAQUE);

        if (ret_sht35 == 0) {
            sht31_convert(temp_raw, hum_raw, &temp_c, &hum_pct);
            wet_bulb = sht31_wet_bulb((float)temp_c, (float)hum_pct);

            if (wet_bulb < 30) {
                led_on();
            } else {
                led_off();
            }

            if (temp_c < 0) {
                temp_abs = -temp_c;
                LCD_ShowChar(45, 0, '-', OPAQUE, CYAN);
                LCD_ShowNum(53, 0, (u16)temp_abs, 3, CYAN);
            } else {
                LCD_ShowNum(45, 0, (u16)temp_c, 3, CYAN);
            }

            LCD_ShowChar(80, 0, 'C', OPAQUE, CYAN);

            LCD_ShowNum(75, 15, (u16)hum_pct, 3, CYAN);
            LCD_ShowChar(102, 15, '%', OPAQUE, CYAN);

            LCD_ShowNum1(75, 30, wet_bulb, 4, GREEN);
            LCD_ShowChar(115, 30, 'C', OPAQUE, GREEN);

            if (temp_c > temp_high) {
                LCD_ShowStr(0, 45, (u8 *)"Temp too high", RED, OPAQUE);
            } else if (temp_c < temp_low) {
                LCD_ShowStr(0, 45, (u8 *)"Temp too low", BLUE, OPAQUE);
            } else {
                LCD_ShowStr(0, 45, (u8 *)"Temp normal", GREEN, OPAQUE);
            }
        } else {
            LCD_ShowStr(0, 45, (u8 *)"Error:", RED, OPAQUE);
        }

        if (ret_vind == 0) {
            vind_read_degrees(vind_raw, &vind_deg);
            LCD_ShowNum(0, 60, vind_deg, 3, CYAN);
        } else {
            LCD_ShowStr(120, 60, (u8 *)"Error", RED, OPAQUE);
        }

        LCD_Wait_On_Queue();

        ////////////////////////////////// motor logik //////////////////////////////////
        delay_ms(1000);
        grader = vind_deg;

        if (grader != bGrader) {
            bGrader = grader;

            if (grader >= 0 && grader < 45) {
                onskatLage = 1;
            }

            if (grader >= 45 && grader < 90) {
                onskatLage = 2;
            }

            if (grader >= 90 && grader < 135) {
                onskatLage = 3;
            }

            if (grader >= 135 && grader < 180) {
                onskatLage = 4;
            }

            if (grader >= 180 && grader < 225) {
                onskatLage = 5;
            }

            if (grader >= 225 && grader < 270) {
                onskatLage = 6;
            }

            if (grader >= 270 && grader < 315) {
                onskatLage = 7;
            }

            if (grader >= 315 && grader < 360) {
                onskatLage = 8;
            }

            if (onskatLage != nuvarandeLage) {
                rotera(onskatLage, nuvarandeLage);
                nuvarandeLage = onskatLage;
            }
        }

        ////////////////////////////////////////////////////////////////////////////////
    }
}

// FUNKTIONER SOM ANVÄNDS

void rotera(int pinMal, int nuvarandeLage) {

    int vinkel = 5;

    switch (pinMal) {

        case 1:
            if (pinMal < nuvarandeLage) {
                do {
                    T1setPWMch2((7 / 100.0) * 16000);
                    T1setPWMch3(0);

                    vinkel = gpio_input_bit_get(GPIOB, GPIO_PIN_11);
                } while (vinkel == 0);

                T1setPWMch2(1);
                T1setPWMch3(1);
                break;
            } else {
                do {
                    T1setPWMch3((7 / 100.0) * 16000);
                    T1setPWMch2(0);

                    vinkel = gpio_input_bit_get(GPIOB, GPIO_PIN_11);
                } while (vinkel == 0);

                T1setPWMch2(1);
                T1setPWMch3(1);
            }
            break;

        case 8:
            if (pinMal < nuvarandeLage) {
                do {
                    T1setPWMch2((7 / 100.0) * 16000);
                    T1setPWMch3(0);

                    vinkel = gpio_input_bit_get(GPIOA, GPIO_PIN_5);
                } while (vinkel == 0);

                T1setPWMch2(1);
                T1setPWMch3(1);
                break;
            } else {
                do {
                    T1setPWMch3((7 / 100.0) * 16000);
                    T1setPWMch2(0);

                    vinkel = gpio_input_bit_get(GPIOA, GPIO_PIN_5);
                } while (vinkel == 0);

                T1setPWMch2(1);
                T1setPWMch3(1);
            }
            break;

        case 7:
            if (pinMal < nuvarandeLage) {
                do {
                    T1setPWMch2((7 / 100.0) * 16000);
                    T1setPWMch3(0);

                    vinkel = gpio_input_bit_get(GPIOB, GPIO_PIN_8);
                } while (vinkel == 0);

                T1setPWMch2(1);
                T1setPWMch3(1);
                break;
            } else {
                do {
                    T1setPWMch3((7 / 100.0) * 16000);
                    T1setPWMch2(0);

                    vinkel = gpio_input_bit_get(GPIOB, GPIO_PIN_8);
                } while (vinkel == 0);

                T1setPWMch2(1);
                T1setPWMch3(1);
            }
            break;

        case 6:
            if (pinMal < nuvarandeLage) {
                do {
                    T1setPWMch2((7 / 100.0) * 16000);
                    T1setPWMch3(0);

                    vinkel = gpio_input_bit_get(GPIOA, GPIO_PIN_7);
                } while (vinkel == 0);

                T1setPWMch2(1);
                T1setPWMch3(1);
                break;
            } else {
                do {
                    T1setPWMch3((7 / 100.0) * 16000);
                    T1setPWMch2(0);

                    vinkel = gpio_input_bit_get(GPIOA, GPIO_PIN_7);
                } while (vinkel == 0);

                T1setPWMch2(1);
                T1setPWMch3(1);
            }
            break;

        case 5:
            if (pinMal < nuvarandeLage) {
                do {
                    T1setPWMch2((7 / 100.0) * 16000);
                    T1setPWMch3(0);

                    vinkel = gpio_input_bit_get(GPIOB, GPIO_PIN_5);
                } while (vinkel == 0);

                T1setPWMch2(1);
                T1setPWMch3(1);
                break;
            } else {
                do {
                    T1setPWMch3((7 / 100.0) * 16000);
                    T1setPWMch2(0);

                    vinkel = gpio_input_bit_get(GPIOB, GPIO_PIN_5);
                } while (vinkel == 0);

                T1setPWMch2(1);
                T1setPWMch3(1);
            }
            break;

        case 4:
            if (pinMal < nuvarandeLage) {
                do {
                    T1setPWMch2((7 / 100.0) * 16000);
                    T1setPWMch3(0);

                    vinkel = gpio_input_bit_get(GPIOB, GPIO_PIN_4);
                } while (vinkel == 0);

                T1setPWMch2(1);
                T1setPWMch3(1);
                break;
            } else {
                do {
                    T1setPWMch3((7 / 100.0) * 16000);
                    T1setPWMch2(0);

                    vinkel = gpio_input_bit_get(GPIOB, GPIO_PIN_4);
                } while (vinkel == 0);

                T1setPWMch2(1);
                T1setPWMch3(1);
            }
            break;

        case 2:
            if (pinMal < nuvarandeLage) {
                do {
                    T1setPWMch2((7 / 100.0) * 16000);
                    T1setPWMch3(0);

                    vinkel = gpio_input_bit_get(GPIOB, GPIO_PIN_12);
                } while (vinkel == 0);

                T1setPWMch2(1);
                T1setPWMch3(1);
                break;
            } else {
                do {
                    T1setPWMch3((7 / 100.0) * 16000);
                    T1setPWMch2(0);

                    vinkel = gpio_input_bit_get(GPIOB, GPIO_PIN_12);
                } while (vinkel == 0);

                T1setPWMch2(1);
                T1setPWMch3(1);
            }
            break;

        case 3:
            if (pinMal < nuvarandeLage) {
                do {
                    T1setPWMch2((7 / 100.0) * 16000);
                    T1setPWMch3(0);

                    vinkel = gpio_input_bit_get(GPIOA, GPIO_PIN_8);
                } while (vinkel == 0);

                T1setPWMch2(1);
                T1setPWMch3(1);
                break;
            } else {
                do {
                    T1setPWMch3((7 / 100.0) * 16000);
                    T1setPWMch2(0);

                    vinkel = gpio_input_bit_get(GPIOA, GPIO_PIN_8);
                } while (vinkel == 0);

                T1setPWMch2(1);
                T1setPWMch3(1);
            }
            break;
    }
}

static void delay_ms(int ms) {
    while (ms > 0) {
        while (!t5expq()) {
            /* stay here until 1 ms has passed */
        }
        ms--;
    }
}
