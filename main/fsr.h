
#pragma once  // Prevents the header file from being included multiple times

void fsr_init();  // Initializes the FSR sensor
int fsr_read();   // Reads and returns the FSR sensor value
#include "esp_adc/adc_oneshot.h"

adc_oneshot_unit_handle_t fsr_get_adc_handle(); // Shares ADC with other sensors
