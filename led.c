#include "gd32vf103.h"
#include "led.h"

void led_init(void){

    rcu_periph_clock_enable(RCU_GPIOB);
    gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_1);
}

void led_on(void)
{
    gpio_bit_set(GPIOB, GPIO_PIN_1);
}

void led_off(void)
{
    gpio_bit_reset(GPIOB, GPIO_PIN_1);
}
