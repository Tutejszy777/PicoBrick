/*
runs a rgb led and switches colors
*/

#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "ws2812.pio.h" // file in same folder via 
// pico_generate_pio_header(adc_dma_capture ${CMAKE_CURRENT_LIST_DIR}/ws2812.pio)

#define RGB_PIN 6
#define BRIGHTNESS 50

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
    pio_sm_put_blocking(pio0, 0, color << 8u); // state 0 to 3
    sleep_us(50);                              // better to have xd
}

int main()
{
    stdio_init_all();

    ws2812_init(RGB_PIN);

    while (true)
    {

        // Red
        ws2812_set_color(255, 0, 0, BRIGHTNESS);
        sleep_ms(1000);

        // Green
        ws2812_set_color(0, 255, 0, BRIGHTNESS);
        sleep_ms(1000);

        // Blue
        ws2812_set_color(0, 0, 255, BRIGHTNESS);
        sleep_ms(1000);

        // Yellow (Red + Green)
        ws2812_set_color(255, 255, 0, BRIGHTNESS);
        sleep_ms(1000);

        // Cyan (Green + Blue)
        ws2812_set_color(0, 255, 255, BRIGHTNESS);
        sleep_ms(1000);

        // Magenta (Red + Blue)
        ws2812_set_color(255, 0, 255, BRIGHTNESS);
        sleep_ms(1000);

        // White (Red + Green + Blue)
        ws2812_set_color(255, 255, 255, BRIGHTNESS);
        sleep_ms(1000);

        // Off
        ws2812_set_color(0, 0, 0, BRIGHTNESS);
        sleep_ms(1000);
    }

    return 0;
}