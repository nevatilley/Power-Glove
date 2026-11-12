
#include <stdio.h>                // Allows us to print sensor readings
#include <stdint.h>               // Provides integer types
#include "freertos/FreeRTOS.h"    // Includes FreeRTOS functions
#include "freertos/task.h"       // Allows delays and time tracking
#include "esp_adc/adc_oneshot.h"  // Allows reading multiple ADC channels
#include "esp_err.h"             // Handles ESP32 errors
#include "fsr.h"                 // Includes FSR functions
#include "flex_sensors.h"        // Includes flex sensor functions

// Sensor testing settings
#define SENSOR_COUNT 6          // Total number of sensors
#define AVERAGE_COUNT 5         // Readings used for averaging
#define SAMPLE_DELAY 20         // Time between readings in milliseconds

extern "C" void app_main() { // Main ESP32 function

    // Initialize the first FSR and flex sensor
    fsr_init(); // Sets up ADC1 and FSR 1 on GPIO34
    adc_oneshot_unit_handle_t adc = fsr_get_adc_handle(); // Shares ADC1
    flex_sensors_init(adc); // Sets up Flex 1 on GPIO32

    // Create settings for the additional sensors
    adc_oneshot_chan_cfg_t config = {};
    config.bitwidth = ADC_BITWIDTH_12; // Uses 12-bit readings (0-4095)
    config.atten = ADC_ATTEN_DB_12; // Sets ADC input range

    // Additional sensors and their ADC channels
    const adc_channel_t extra_channels[4] = {
        ADC_CHANNEL_7, // FSR 2 - GPIO35
        ADC_CHANNEL_5, // Flex 2 - GPIO33
        ADC_CHANNEL_0, // Flex 3 - GPIO36
        ADC_CHANNEL_3  // Flex 4 - GPIO39
    };

    // Configure the four additional sensor inputs
    for (int i = 0; i < 4; i++) {
        ESP_ERROR_CHECK(adc_oneshot_config_channel(
            adc, extra_channels[i], &config));
    }

    // Store recent readings for each sensor
    int samples[SENSOR_COUNT][AVERAGE_COUNT] = {};

    int sample_index = 0; // Current position in the arrays
    int samples_taken = 0; // Number of valid readings stored

    // Print column names for Excel testing
    printf("Time_ms,FSR1_Raw,FSR1_Avg,FSR2_Raw,FSR2_Avg,"
           "Flex1_Raw,Flex1_Avg,Flex2_Raw,Flex2_Avg,"
           "Flex3_Raw,Flex3_Avg,Flex4_Raw,Flex4_Avg\n");

    while (true) { // Continuously collect sensor readings

        int raw[SENSOR_COUNT] = {}; // Stores current readings
        int avg[SENSOR_COUNT] = {}; // Stores averaged readings

        // Read both FSR sensors
        raw[0] = fsr_read(); // FSR 1 - GPIO34

        ESP_ERROR_CHECK(adc_oneshot_read(
            adc, ADC_CHANNEL_7, &raw[1])); // FSR 2 - GPIO35

        // Read all four flex sensors
        raw[2] = flex_sensor_read(); // Flex 1 - GPIO32

        ESP_ERROR_CHECK(adc_oneshot_read(
            adc, ADC_CHANNEL_5, &raw[3])); // Flex 2 - GPIO33

        ESP_ERROR_CHECK(adc_oneshot_read(
            adc, ADC_CHANNEL_0, &raw[4])); // Flex 3 - GPIO36

        ESP_ERROR_CHECK(adc_oneshot_read(
            adc, ADC_CHANNEL_3, &raw[5])); // Flex 4 - GPIO39

        // Save the newest readings for every sensor
        for (int i = 0; i < SENSOR_COUNT; i++) {
            samples[i][sample_index] = raw[i];
        }

        // Count valid samples until arrays are full
        if (samples_taken < AVERAGE_COUNT) {
            samples_taken++;
        }

        // Update the position for the next reading
        sample_index = (sample_index + 1) % AVERAGE_COUNT;

        // Calculate the average for each sensor
        for (int i = 0; i < SENSOR_COUNT; i++) {
            int sum = 0;

            // Add the most recent readings together
            for (int j = 0; j < samples_taken; j++) {
                sum += samples[i][j];
            }

            avg[i] = sum / samples_taken; // Calculate average
        }

        // Record time since the ESP32 started
        uint32_t time_ms = (uint32_t)
            (xTaskGetTickCount() * portTICK_PERIOD_MS);

        // Print the timestamp
        printf("%lu", (unsigned long)time_ms);

        // Print raw and averaged values for all sensors
        for (int i = 0; i < SENSOR_COUNT; i++) {
            printf(",%d,%d", raw[i], avg[i]);
        }

        printf("\n"); // Start a new line for next sample

        // Wait approximately 20 milliseconds
        vTaskDelay(pdMS_TO_TICKS(SAMPLE_DELAY));
    }
}
