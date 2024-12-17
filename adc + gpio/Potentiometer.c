#include "hardware/adc.h"
#include "pico/stdlib.h"
#include <stdio.h>

#define ADC_PIN 26
#define MAX_ADC_VALUE 4095 // max 12 bit value of potentiometer
#define MAX_BRIGHTNESS 100

void adc_init_brightness()
{
    adc_init();
    adc_gpio_init(ADC_PIN);
    adc_select_input(0);
}

uint8_t adc_get_brightness(void)
{
    uint16_t adc_value = adc_read(); // (0-4095)
    printf("ADC Value: %d\n", adc_value);
    return (uint8_t)((adc_value * MAX_BRIGHTNESS) / MAX_ADC_VALUE); //  0 to 100%
}

int main()
{

    stdio_init_all();
    adc_init_brightness();

    while (true)
    {
        uint8_t brit = adc_get_brightness();
        printf("Brightness: %d%%\n", brit);

        sleep_ms(5000);
    }

    return 0;
}