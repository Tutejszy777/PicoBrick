// an algorithm by AI to read from DHT11

#include <stdio.h>
#include "pico/stdlib.h"

#define DHT_PIN 4
#define TEMP_THRESHOLD 30.0
#define START_SIGNAL_DELAY 18000
#define RESPONSE_WAIT_TIME 40
#define HIGH_SIGNAL_THRESHOLD 40

//  read data from DHT11 with help of AI
bool read_dht11(float* temperature, float* humidity) {
    uint8_t data[5] = {0};

    // Send start signal
    gpio_init(DHT_PIN);
    gpio_set_dir(DHT_PIN, GPIO_OUT);
    gpio_put(DHT_PIN, 0);
    sleep_us(START_SIGNAL_DELAY);
    gpio_put(DHT_PIN, 1);
    sleep_us(40); // Switch to input mode quickly
    gpio_set_dir(DHT_PIN, GPIO_IN);

    // Wait for response from DHT11
    if (gpio_get(DHT_PIN) == 1) return false;
    sleep_us(RESPONSE_WAIT_TIME);
    if (gpio_get(DHT_PIN) == 0) return false;

    // Read 40 bits of data
    for (int i = 0; i < 40; i++) {
        // Wait for the LOW signal
        while (gpio_get(DHT_PIN) == 0);

        // Measure the HIGH signal duration
        uint32_t start_time = time_us_32();
        while (gpio_get(DHT_PIN) == 1);
        uint32_t duration = time_us_32() - start_time;

        // Store bit based on signal duration
        int byte_idx = i / 8;
        int bit_idx = 7 - (i % 8);
        if (duration > HIGH_SIGNAL_THRESHOLD) {
            data[byte_idx] |= (1 << bit_idx);
        }
    }

    // Verify checksum
    uint8_t checksum = data[0] + data[1] + data[2] + data[3];
    if (checksum != data[4]) return false;

    // Extract humidity and temperature
    *humidity = data[0] + (data[1] * 0.1);
    *temperature = data[2] + (data[3] * 0.1);

    return true;
}

int main() {
    stdio_init_all();

    while (true) {
        float temperature = 0.0, humidity = 0.0;

        if (read_dht11(&temperature, &humidity)) {
            printf("Temperature: %.1f°C, Humidity: %.1f%%\n", temperature, humidity);
        } else {
            printf("Failed to read from DHT11 sensor\n");
        }

        sleep_ms(2000); // Wait 2 seconds before next reading
    }

    return 0;
}