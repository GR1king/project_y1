#include "gd32vf103.h"
#include "led.h"

void led_init(void){

    rcu_periph_clock_enable(RCU_GPIOA);
    gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_1);
}

void led_on(void)
{
    gpio_bit_set(GPIOA, GPIO_PIN_1);
}

void led_off(void)
{
    gpio_bit_reset(GPIOA, GPIO_PIN_1);
}