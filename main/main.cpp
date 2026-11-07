
#include <stdio.h>              // Allows us to print sensor readings
#include "freertos/FreeRTOS.h"  // Includes FreeRTOS functions
#include "freertos/task.h"     // Allows us to use delays
#include "fsr.h"               // Includes FSR functions
#include "flex_sensors.h"      // Includes flex sensor functions

extern "C" void app_main() {  // Main function for the ESP32
    fsr_init(); // Initializes the FSR sensor
    flex_sensors_init(fsr_get_adc_handle()); // Initializes the flex sensor using ADC1

    while (true) { // Continuously reads both sensors
        int fsr_value = fsr_read(); // Reads FSR pressure
        int flex_value = flex_sensor_read(); // Reads finger bending

        printf("FSR: %d | Flex: %d\n", fsr_value, flex_value); // Displays both readings

        vTaskDelay(pdMS_TO_TICKS(200)); // Waits 200 milliseconds
    }
}
