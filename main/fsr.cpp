
#include "fsr.h"                  // Includes our FSR functions
#include "esp_adc/adc_oneshot.h"  // Allows the ESP32 to read analog signals
#include "esp_err.h"             // Handles ESP32 errors

static adc_oneshot_unit_handle_t fsr_adc; // Stores the ADC handle for the FSR

void fsr_init() { // Sets up the FSR sensor
    adc_oneshot_unit_init_cfg_t unit_config = {}; // Creates ADC settings
    unit_config.unit_id = ADC_UNIT_1; // Selects ADC1 on the ESP32

    // Initializes ADC1 using the selected settings
    ESP_ERROR_CHECK(adc_oneshot_new_unit(&unit_config, &fsr_adc));

    adc_oneshot_chan_cfg_t channel_config = {}; // Creates channel settings
    channel_config.bitwidth = ADC_BITWIDTH_12; // Sets resolution to 12 bits (0-4095)
    channel_config.atten = ADC_ATTEN_DB_12; // Sets the input voltage range

    // Configures ADC1 Channel 6, which connects to GPIO34
    ESP_ERROR_CHECK(adc_oneshot_config_channel(
        fsr_adc, ADC_CHANNEL_6, &channel_config));
}


int fsr_read() { // Reads the pressure detected by the FSR
    int fsr_value = 0; // Stores the sensor reading

    // Reads the analog voltage from GPIO34
    ESP_ERROR_CHECK(adc_oneshot_read(
        fsr_adc, ADC_CHANNEL_6, &fsr_value));

    return fsr_value; // Returns the raw reading (0-4095)
}

adc_oneshot_unit_handle_t fsr_get_adc_handle() {
    return fsr_adc; // Returns the initialized ADC handle
}
