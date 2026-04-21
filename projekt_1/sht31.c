//Väntar med att lägga in sht31.c


#include "sht31.h"
#include "drivers.h"
#include "math.h"
/* sensor I2C address */
#define SHT31_ADDR 0x45
#define I2C_TIMEOUT 200000
/* wait for a number of milliseconds */

/* set up I2C on PB6 = SCL and PB7 = SDA */
void sht31_init_setup(void) {
    rcu_periph_clock_enable(RCU_I2C0);   /* turn on clock for I2C0 */
    rcu_periph_clock_enable(RCU_GPIOB);  /* turn on clock for port B */
    rcu_periph_clock_enable(RCU_AF);     /* turn on alternate function clock */

    /* set PB6 and PB7 as I2C pins */
    gpio_init(GPIOB, GPIO_MODE_AF_OD, GPIO_OSPEED_50MHZ, GPIO_PIN_6 | GPIO_PIN_7);

    /* reset and configure I2C */
    i2c_deinit(I2C0);
    i2c_clock_config(I2C0, 100000, I2C_DTCY_2);   /* 100 kHz */
    i2c_mode_addr_config(I2C0, I2C_I2CMODE_ENABLE, I2C_ADDFORMAT_7BITS, 0x00);
    i2c_enable(I2C0);                             /* turn on I2C */
    i2c_ack_config(I2C0, I2C_ACK_ENABLE);        /* enable ACK */
}

static void wait_ms_sensor(int ms) {
    while (ms > 0) {
        while (!t5expq()) {
        }
        ms--;
    }
}

static int wait_flag_set(uint32_t port, i2c_flag_enum flag) {
    uint32_t timeout = I2C_TIMEOUT;
    while (!i2c_flag_get(port, flag)) {
        if (--timeout == 0) {
            return -1;
        }
    }
    return 0;
}

static int wait_bus_free(uint32_t port) {
    uint32_t timeout = I2C_TIMEOUT;
    while (i2c_flag_get(port, I2C_FLAG_I2CBSY)) {
        if (--timeout == 0) {
            return -1;
        }
    }
    return 0;
}

/* send the 2-byte command that tells the sensor to measure */
static void sht31_write_command(uint8_t cmd1, uint8_t cmd2) {
    while (i2c_flag_get(I2C0, I2C_FLAG_I2CBSY)) {
        /* wait until I2C bus is free */
    }

    i2c_start_on_bus(I2C0);                           /* send START */
    while (!i2c_flag_get(I2C0, I2C_FLAG_SBSEND)) {
    }

    i2c_master_addressing(I2C0, SHT31_ADDR << 1, I2C_TRANSMITTER); /* send address + write */
    while (!i2c_flag_get(I2C0, I2C_FLAG_ADDSEND)) {
    }
    i2c_flag_clear(I2C0, I2C_FLAG_ADDSEND);

    i2c_data_transmit(I2C0, cmd1);                   /* send first byte */
    while (!i2c_flag_get(I2C0, I2C_FLAG_TBE)) {
    }

    i2c_data_transmit(I2C0, cmd2);                   /* send second byte */
    while (!i2c_flag_get(I2C0, I2C_FLAG_TBE)) {
    }

    i2c_stop_on_bus(I2C0);                           /* send STOP */
    while (I2C_CTL0(I2C0) & I2C_CTL0_STOP) {
    }
}

/* read bytes back from the sensor */
static void sht31_read_data(uint8_t size, uint8_t *data) {
    i2c_start_on_bus(I2C0);                          /* send START */
    while (!i2c_flag_get(I2C0, I2C_FLAG_SBSEND)) {
    }

    i2c_master_addressing(I2C0, SHT31_ADDR << 1, I2C_RECEIVER);   /* send address + read */
    while (!i2c_flag_get(I2C0, I2C_FLAG_ADDSEND)) {
    }
    i2c_flag_clear(I2C0, I2C_FLAG_ADDSEND);

    i2c_ack_config(I2C0, I2C_ACK_ENABLE);           /* ACK bytes while reading */

    for (int i = 0; i < size; i++) {
        if (i == size - 1) {
            i2c_ack_config(I2C0, I2C_ACK_DISABLE);  /* last byte: no ACK */
        }

        while (!i2c_flag_get(I2C0, I2C_FLAG_RBNE)) {
            /* wait until one byte arrives */
        }

        *data++ = i2c_data_receive(I2C0);           /* store received byte */
    }

    i2c_stop_on_bus(I2C0);                          /* send STOP */
    while (I2C_CTL0(I2C0) & I2C_CTL0_STOP) {
    }

    i2c_ack_config(I2C0, I2C_ACK_ENABLE);          /* turn ACK on again */
}

/* read one full measurement from the sensor */
int sht31_read(uint16_t *temp_raw, uint16_t *hum_raw) {
    uint8_t data[5];    /* sensor sends 5 bytes */
    uint16_t t_raw;
    uint16_t h_raw;

    if ((temp_raw == 0) || (hum_raw == 0)) {
        return -1;      /* bad input */
    }

    sht31_init_setup();

    sht31_write_command(0x24, 0x00);  /* tell sensor to measure */
    wait_ms_sensor(20);               /* wait for measurement */

    sht31_read_data(5, data);         /* read 5 bytes from sensor */

    /* make 16-bit raw temperature from first 2 bytes */
    t_raw = ((uint16_t)data[0] << 8) | data[1];

    /* make 16-bit raw humidity from bytes 4 and 5 */
    h_raw = ((uint16_t)data[3] << 8) | data[4];

    *temp_raw = t_raw;   /* return raw temperature */
    *hum_raw  = h_raw;   /* return raw humidity */

    return 0;            /* success */
}

/* convert raw values into normal values */
void sht31_convert(uint16_t temp_raw, uint16_t hum_raw, int *temp_c, int *hum_pct) {
    *temp_c  = -45 + (int)((175L * temp_raw) / 65535L);  /* convert to Celsius */
    *hum_pct = (int)((100L * hum_raw) / 65535L);         /* convert to percent */

    if (*hum_pct < 0) *hum_pct = 0;      /* keep humidity above 0 */
    if (*hum_pct > 100) *hum_pct = 100;  /* keep humidity below 100 */
}

float sht31_wet_bulb(float temp_c, float hum_pct){
    float wbt;
    wbt = temp_c * atan(0.151977 * sqrt(hum_pct + 8.313659)) + atan(temp_c + hum_pct) - atan(hum_pct - 1.676331) + 0.00391838 * pow(hum_pct, 1.5) * atan(0.023101 * hum_pct) - 4.686035;
    return wbt;
}
