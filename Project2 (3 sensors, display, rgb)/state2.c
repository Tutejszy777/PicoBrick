/*
Future work connect thermometer and display info to OLED display
brightness of rgb controled used potentiometer and the color depending on the light emerged on light sensor
*/
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"
#include "hardware/pio.h"
#include "ws2812.pio.h"

#define RGB_PIN 6
#define POTENTIOMETER_PIN 26 // channel 0
#define POT_CHANNEL 0
#define SENSOR_PIN 27 // channel 1
#define SENS_CHANNEL 1
#define MAX_ADC_VALUE 4095 // 12 bit ADC
#define MAX_BRIGHTNESS 100

void init_hardware()
{
    adc_init();
    adc_gpio_init(SENSOR_PIN);
    adc_gpio_init(POTENTIOMETER_PIN);
}

uint16_t read_adc_channel(uint channel)
{
    adc_select_input(channel);
    return adc_read();
}

uint8_t adc_get_brightness()
{
    uint16_t adc_value = read_adc_channel(POT_CHANNEL);
    return (adc_value * MAX_BRIGHTNESS) / MAX_ADC_VALUE;
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

int main()
{
    stdio_init_all();
    init_hardware();
    ws2812_init(RGB_PIN);

    while (true)
    {
        uint16_t analog_value = read_adc_channel(SENS_CHANNEL);
        uint16_t brightness = adc_get_brightness();

        printf("LDR Value: %d\n", analog_value);
        printf("Percentage of brightness: %d\n", brightness);

        if (analog_value < 200)
        { // red
            ws2812_set_color(255, 0, 0, brightness);
        }
        else if (analog_value >= 200 && analog_value < 400)
        { // green
            ws2812_set_color(0, 255, 0, brightness);
        }
        else if (analog_value >= 400 && analog_value < 600)
        { // blue
            ws2812_set_color(0, 0, 255, brightness);
        }
        else if (analog_value >= 600 && analog_value < 800)
        { // Cyan (Green + Blue)
            ws2812_set_color(0, 255, 255, brightness);
        }
        else if (analog_value >= 800 && analog_value < 1000)
        { // Magenta (Red + Blue)
            ws2812_set_color(255, 0, 255, brightness);
        }
        else
        { // White (Red + Green + Blue)
            ws2812_set_color(255, 255, 255, brightness);
        }
        sleep_ms(1000);
    }
    return 0;
}