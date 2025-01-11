#include <hardware/i2c.h>
#include <stdio.h>

void setup(){
    i2c_init(i2c0, 100 * 1000) // frequency
    gpio_set_function(4, GPIO_FUNC_I2C); // SDA
    gpio_set_function(5, GPIO_FUNC_I2C); // SCL
}

void i2c_read() {
    uint8_t = memory_address = 0x00; // to start read from
    uint8_t = read_data[2];

    i2c_write_blocking(i2c0, 0x50, &memory_address, 1, true); // sends address to read ( 0x50 address of EEPROM) (true means repeated)

    i2c_read_blocking(i2c0, 0x50, read_data, 2, true); //true for send stop condition

    printf(" 0x%02X ,, 0x%02X \n", read_data[0], read_data[1]);

}

int main() {
    setup();
    i2c_read();

    return 0;
}