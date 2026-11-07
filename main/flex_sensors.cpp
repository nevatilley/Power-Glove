
#include "flex_sensors.h"        // Includes flex sensor functions
#include "esp_err.h"            // Handles ESP32 errors

static adc_oneshot_unit_handle_t flex_adc = nullptr; // Stores shared ADC handle

void flex_sensors_init(adc_oneshot_unit_handle_t adc) {
    flex_adc = adc; // Uses the ADC already initialized by the FSR

    adc_oneshot_chan_cfg_t config = {}; // Creates channel settings
    config.bitwidth = ADC_BITWIDTH_12; // Sets readings from 0 to 4095
    config.atten = ADC_ATTEN_DB_12; // Sets the input voltage range

    // Configures ADC1 Channel 4, connected to GPIO32
    ESP_ERROR_CHECK(adc_oneshot_config_channel(
        flex_adc, ADC_CHANNEL_4, &config));
}

int flex_sensor_read() {
    if (flex_adc == nullptr) return -1; // Checks initialization

    int flex_value = 0; // Stores the sensor reading

    // Reads the analog voltage from GPIO32
    ESP_ERROR_CHECK(adc_oneshot_read(
        flex_adc, ADC_CHANNEL_4, &flex_value));

    return flex_value; // Returns the raw sensor reading
}
