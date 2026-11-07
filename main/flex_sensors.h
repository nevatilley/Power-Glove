//FOR ONE FINGER ONLY

#pragma once  // Prevents the header from being included multiple times

#include "esp_adc/adc_oneshot.h" // Includes ADC functions

void flex_sensors_init(adc_oneshot_unit_handle_t adc); // Sets up the flex sensor
int flex_sensor_read(); // Reads and returns the flex sensor value
