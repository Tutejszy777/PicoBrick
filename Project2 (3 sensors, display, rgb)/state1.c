//only lightsens and rgb done on this step

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"
#include "hardware/pio.h"
#include "ws2812.pio.h"

#define RGB_PIN 6
#define SENSOR_PIN 27
#define BRIGHTNESS 50

void init_hardware()
{
    adc_init();
    adc_gpio_init(SENSOR_PIN);
    adc_select_input(1);
}

void ws2812_init(uint pin)
{
    PIO pio = pio0;                                      //  instance
    int sm = 0;                                          // machine state 0
    uint offset = pio_add_program(pio, &ws2812_program); // load program
    ws2812_program_init(pio, sm, offset, pin, 800000, false);
}

// color
void ws2812_set_color(uint8_t r, uint8_t g, uint8_t b, float brightness)
{
    r = (uint8_t)(r * (brightness / 100.0f));
    g = (uint8_t)(g * (brightness / 100.0f));
    b = (uint8_t)(b * (brightness / 100.0f));

    // 24bit color in 32
    uint32_t color = ((uint32_t)g << 16) | ((uint32_t)r << 8) | b;

    // send
    pio_sm_put_blocking(pio0, 0, color << 8u); // state 0 to 3, move by 8
    sleep_us(50);                              // better to have xd
}

uint16_t read_ldr_value()
{
    return adc_read();
}

int main()
{
    stdio_init_all();
    init_hardware();
    ws2812_init(RGB_PIN);

    while (true)
    {
        ws2812_set_color(0, 0, 0, BRIGHTNESS);
        uint16_t analog_value = adc_read();

        printf("LDR Value: %d\n", analog_value);

        if (analog_value < 200)
        { // red
            ws2812_set_color(255, 0, 0, BRIGHTNESS);
        }
        else if (analog_value >= 200 && analog_value < 400)
        { // green
            ws2812_set_color(0, 255, 0, BRIGHTNESS);
        }
        else if (analog_value >= 400 && analog_value < 600)
        { // blue
            ws2812_set_color(0, 0, 255, BRIGHTNESS);
        }
        else if (analog_value >= 600 && analog_value < 800)
        { // Cyan (Green + Blue)
            ws2812_set_color(0, 255, 255, BRIGHTNESS);
        }
        else if (analog_value >= 800 && analog_value < 1000)
        { // Magenta (Red + Blue)
            ws2812_set_color(255, 0, 255, BRIGHTNESS);
        }
        else
        { // White (Red + Green + Blue)
            ws2812_set_color(255, 255, 255, BRIGHTNESS);
        }
        sleep_ms(1000);
    }
    return 0;
}