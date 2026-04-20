#include "vind.h"
#include "gd32vf103.h"
#include <math.h>

volatile uint8_t read_flag = 0;

void i2c_read_register(int32_t port, uint8_t addr, uint8_t reg, uint8_t size, uint8_t *data) {
    i2c_start_on_bus(port);
    while (!i2c_flag_get(port, I2C_FLAG_SBSEND));

    i2c_master_addressing(port, addr << 1, I2C_TRANSMITTER);
    while (!i2c_flag_get(port, I2C_FLAG_ADDSEND));
    i2c_flag_clear(port, I2C_FLAG_ADDSEND);

    i2c_data_transmit(port, reg);
    while (!i2c_flag_get(port, I2C_FLAG_TBE));

    i2c_start_on_bus(port);
    while (!i2c_flag_get(port, I2C_FLAG_SBSEND));

    i2c_master_addressing(port, addr << 1, I2C_RECEIVER);
    while (!i2c_flag_get(port, I2C_FLAG_ADDSEND));
    i2c_flag_clear(port, I2C_FLAG_ADDSEND);

    i2c_ack_config(port, I2C_ACK_ENABLE);

    for (int i = 0; i < size; i++) {
        if (i == size - 1) {
            i2c_ack_config(port, I2C_ACK_DISABLE);
        }
        while (!i2c_flag_get(port, I2C_FLAG_RBNE));
        *data++ = i2c_data_receive(port);
    }

    i2c_stop_on_bus(port);
    while (I2C_CTL0(port) & I2C_CTL0_STOP);
}

void i2c_write_register(int32_t port, uint8_t addr, uint8_t reg, uint8_t size, uint8_t *data) {
    while (i2c_flag_get(port, I2C_FLAG_I2CBSY));

    i2c_start_on_bus(port);
    while (!i2c_flag_get(port, I2C_FLAG_SBSEND));

    i2c_master_addressing(port, addr << 1, I2C_TRANSMITTER);
    while (!i2c_flag_get(port, I2C_FLAG_ADDSEND));
    i2c_flag_clear(port, I2C_FLAG_ADDSEND);

    i2c_data_transmit(port, reg);
    while (!i2c_flag_get(port, I2C_FLAG_TBE));

    for (int i = 0; i < size; i++) {
        i2c_data_transmit(port, *data++);
        while (!i2c_flag_get(port, I2C_FLAG_TBE));
    }

    i2c_stop_on_bus(port);
    while (I2C_CTL0(port) & I2C_CTL0_STOP);
}

void timer_config(uint32_t timer_periph, uint32_t time_interval_ms) {
    timer_parameter_struct timer_initpara;
    rcu_periph_clock_enable(RCU_TIMER5);
    timer_deinit(timer_periph);
    timer_initpara.prescaler        = 10799;
    timer_initpara.alignedmode      = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection = TIMER_COUNTER_UP;
    timer_initpara.period           = (uint32_t)10 * time_interval_ms;
    timer_initpara.clockdivision    = TIMER_CKDIV_DIV1;
    timer_init(timer_periph, &timer_initpara);
    timer_interrupt_enable(timer_periph, TIMER_INT_UP);
    timer_enable(timer_periph);
}

void TIMER5_IRQHandler(void) {
    if (SET == timer_interrupt_flag_get(TIMER5, TIMER_INT_FLAG_UP)) {
        timer_interrupt_flag_clear(TIMER5, TIMER_INT_FLAG_UP);
        read_flag = 1;
    }
}

float getAverageAngle(float angles[], int count) {
    float sumX = 0;
    float sumY = 0;

    for (int i = 0; i < count; i++) {
        float rad = angles[i] * 3.14159265f / 180.0f;
        sumX += cos(rad);
        sumY += sin(rad);
    }

    sumX /= count;
    sumY /= count;

    float avgDeg = atan2(sumY, sumX) * 180.0f / 3.14159265f;
    if (avgDeg < 0) avgDeg += 360.0f;

    return avgDeg;
}
