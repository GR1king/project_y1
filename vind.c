#include "gd32vf103.h"
#include "vind.h"
#include "math.h"

#define I2C_TIMEOUT 200000

void vind_init_setup(void) {
    rcu_periph_clock_enable(RCU_I2C0);
    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_AF);

    gpio_init(GPIOB, GPIO_MODE_AF_OD, GPIO_OSPEED_50MHZ, GPIO_PIN_10 | GPIO_PIN_11);

    i2c_deinit(I2C0);
    i2c_clock_config(I2C0, 400000, I2C_DTCY_2);
    i2c_mode_addr_config(I2C0, I2C_I2CMODE_ENABLE, I2C_ADDFORMAT_7BITS, 0x00);
    i2c_enable(I2C0);
    i2c_ack_config(I2C0, I2C_ACK_ENABLE);
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

static void vind_read_data(uint8_t reg, uint8_t size, uint8_t *data) {
    while (i2c_flag_get(I2C0, I2C_FLAG_I2CBSY)) {
    }

    i2c_start_on_bus(I2C0);
    while (!i2c_flag_get(I2C0, I2C_FLAG_SBSEND)) {
    }

    i2c_master_addressing(I2C0, VIND_SENSOR_ADDR << 1, I2C_TRANSMITTER);
    while (!i2c_flag_get(I2C0, I2C_FLAG_ADDSEND)) {
    }
    i2c_flag_clear(I2C0, I2C_FLAG_ADDSEND);

    i2c_data_transmit(I2C0, reg);
    while (!i2c_flag_get(I2C0, I2C_FLAG_TBE)) {
    }

    i2c_start_on_bus(I2C0);
    while (!i2c_flag_get(I2C0, I2C_FLAG_SBSEND)) {
    }

    i2c_master_addressing(I2C0, VIND_SENSOR_ADDR << 1, I2C_RECEIVER);
    while (!i2c_flag_get(I2C0, I2C_FLAG_ADDSEND)) {
    }
    i2c_flag_clear(I2C0, I2C_FLAG_ADDSEND);

    i2c_ack_config(I2C0, I2C_ACK_ENABLE);

    for (int i = 0; i < size; i++) {
        if (i == size - 1) {
            i2c_ack_config(I2C0, I2C_ACK_DISABLE);
        }

        while (!i2c_flag_get(I2C0, I2C_FLAG_RBNE)) {
        }

        *data++ = i2c_data_receive(I2C0);
    }

    i2c_stop_on_bus(I2C0);
    while (I2C_CTL0(I2C0) & I2C_CTL0_STOP) {
    }

    i2c_ack_config(I2C0, I2C_ACK_ENABLE);
}

int vind_read_raw(uint16_t *vind_raw) {
    uint8_t data[2];
    uint16_t raw;

    if (vind_raw == 0) {
        return -1;
    }

    /* sätt rätt I2C-hastighet innan vindsensorn används */
    vind_init_setup();

    /* AS5600 register 0x0C */
    vind_read_data(0x0C, 2, data);

    raw = ((uint16_t)data[0] << 8) | data[1];

    *vind_raw = raw;

    return 0;
}

void vind_read_degrees(uint16_t vind_raw, int *vind_deg) {
    *vind_deg = (int)((360L * vind_raw) / 4095L);
}

float getAverageAngle(float angles[], int count)
{
    float sumX = 0;
    float sumY = 0;

    for (int i = 0; i < count; i++)
    {
        float rad = angles[i] * 3.14159265  / 180.0;

        sumX += cos(rad);
        sumY += sin(rad);
    }

    sumX /= count;
    sumY /= count;

    float avgRad = atan2(sumY, sumX);
    float avgDeg = avgRad * 180.0 / 3.14159265 ;

    if (avgDeg < 0)
    {
        avgDeg += 360;
    }

    return avgDeg;
}
