#include <hardware/i2c.h>
#include <stdio.h>

void setup(){
    i2c_init(i2c0, 100 * 1000) // frequency
    gpio_set_function(4, GPIO_FUNC_I2C); // SDA
    gpio_set_function(5, GPIO_FUNC_I2C); // SCL
}

void i2c_write (){
    uint8_t data[] = {0x01, 0x02};

    i2c_write_blocking(i2c0, 0x50, data, 2, true); // true for stop condition (2 number of bytes)
}

int main() {
    setup();
    i2c_write();

    return 0;
};
