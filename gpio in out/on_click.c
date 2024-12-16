#include <stdio.h>
#include "pico/stdlib.h"

const uint led_pin = 7;
const uint btn_pin = 10;

int main()
{
    stdio_init_all();

    gpio_init(led_pin);
    gpio_init(btn_pin);
    gpio_set_dir(led_pin, GPIO_OUT);
    gpio_set_dir(btn_pin, GPIO_IN);

    while (true)
    {

        if (gpio_get(btn_pin) == true)
        {
            gpio_put(led_pin, true);
        }
        else
        {
            gpio_put(led_pin, false);
        }

        sleep_ms(50);
    }
}