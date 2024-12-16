#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"

#define RELAY_PIN 7
#define DARK 700

void init_hardware()
{
    adc_init();
    adc_gpio_init(27);
    adc_select_input(1);

    gpio_init(RELAY_PIN);
    gpio_set_dir(RELAY_PIN, GPIO_OUT);
}

uint16_t read_ldr_value()
{
    return adc_read();
}

int main()
{
    stdio_init_all();
    init_hardware();

    while (true)
    {
        uint16_t analog_value = adc_read();

        printf("LDR Value: %d\n", analog_value);

        if (analog_value > DARK)
        {
            gpio_put(RELAY_PIN, 1);
        }
        else
        {
            gpio_put(RELAY_PIN, 0);
        }
        sleep_ms(1000);
    }
}