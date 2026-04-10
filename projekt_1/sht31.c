#include "sht31.h" //Vi använder SHT35

void i2c_init_setup(void) {
    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_I2C0);

    gpio_init(GPIOB, GPIO_MODE_AF_OD, GPIO_OSPEED_50MHZ,GPIO_PIN_6 | GPIO_PIN_7);

    i2c_clock_config(I2C0, 100000, I2C_DTCY_2);
    i2c_mode_addr_config(I2C0, I2C_I2CMODE_ENABLE,
                         I2C_ADDFORMAT_7BITS, 0x00);
    i2c_enable(I2C0);
    i2c_ack_config(I2C0, I2C_ACK_ENABLE);
}

int sh31_read(int16_t *temp_raw, uint16_t *hum_raw) {
    
}





