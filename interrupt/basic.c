#include "pico/stdlib.h"
#include "hardware/irq.h"
#include "hardware/gpio.h"

#define LED_PIN 7   
#define BUTTON_PIN 10 

void gpio_callback(uint gpio, uint32_t events) {
    if (gpio == BUTTON_PIN) {
        gpio_put(LED_PIN, !gpio_get(LED_PIN));
    }
}

int main() {
    stdio_init_all();
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_init(BUTTON_PIN);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_PIN);  

    
    gpio_set_irq_enabled_with_callback(BUTTON_PIN, GPIO_IRQ_EDGE_FALL, true, &gpio_callback);

    while (true) {
        tight_loop_contents();
    }
}
