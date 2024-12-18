/*
init led , on and off
*/

#include <stdio.h>
#include "pico/stdlib.h"

const uint led_pin = 20;

int main()
{
    stdio_init_all();

    gpio_init(led_pin);
    gpio_set_dir(led_pin, GPIO_OUT);

    while (true)
    {
        gpio_put(led_pin, true);
        sleep_ms(100);
        gpio_put(led_pin, false);
        sleep_ms(10);
    }
}