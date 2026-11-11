#include "imu.h"               // Includes IMU functions
#include "esp_adc/adc_oneshot.h"  // Allows the ESP32 to read analog signals
#include "esp_err.h"             // Handles ESP32 errors
#include "esp_log.h"             // Allows the ESP32 to log information
#include "esp_timer.h"           // Allows the ESP32 to use timers

#include "drivers/i2c_master.h"  // Allows the ESP32 to communicate with I2C devices
#include "freertos/FreeRTOS.h"         // Allows the ESP32 to use FreeRTOS
#include "freertos/task.h"             // Allows the ESP32 to use FreeRTOS tasks

/* The IMU (Inertial Measurement Unit) contains two sensors:
 * 1. Gyroscope (measures angular velocity) (degrees per second)
 * 2. Accelerometer (measures linear acceleration) (meters per second squared)
 */

static const char *TAG = "IMU_APP"; // Tag for logging

/* I2C Device Link & Configuration */
i2c_master_dev_handle_t dev_handle; // Handle for the I2C device

void i2c_sensor_init(void){
    ESP_LOGI(TAG, "Initializing I2C Master Bus..."); // Log the initialization of the I2C master bus

    // Configure and allocate the I2C Master Bus
    i2c_master_bus_config_t bus_config = {
        .clk_source = I2C_CLK_SRC_DEFAULT, // Use the default clock source
        .i2c_port = I2C_NUM_0, // Use I2C port 0
        .scl_io_num = I2C_MASTER_SCL_IO, // Set the SCL pin
        .sda_io_num = I2C_MASTER_SDA_IO, // Set the SDA pin
        .glitch_ignore_cnt = 7, // Ignore glitches on the I2C bus
        .flags.enable_internal_pullup = true, // Enable internal pull-up resistors // SET FALSE IF USING EXTERNAL HW PULLUPS
    }
    i2c_master_bus_handle_t bus_handle; // Handle for the I2C bus
    ESP_ERROR_CHECK(i2c_new_master_bus(&bus_config, &bus_handle)); // Create a new I2C master bus

    // Add the MPU6050 device to the I2C bus
    i2c_device_config_t dev_config = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7, // Use 7-bit addressing
        .dev_addr = MPU6050_ADDR, // Set the device address
        .scl_speed_hz = 400000, // Set the SCL speed to 400 kHz
    }
    ESP_ERROR_CHECK(i2c_master_bus_add_device(bus_handle, &dev_config, &dev_handle)); // Add the MPU6050 device to the I2C bus
    ESP_LOGI(TAG, "I2C Bus & Device Initialized Successfully"); // Log successful initialization
}


/* Filter*/

/* IMU */