
#ifndef IMU_H
#define IMU_H

#include <stdint.h>
#include "esp_err.h"

// Stores raw measurements from the MPU-6050
struct IMUReading {
    int16_t accel_x;
    int16_t accel_y;
    int16_t accel_z;

    int16_t gyro_x;
    int16_t gyro_y;
    int16_t gyro_z;
};

// Initializes the IMU
esp_err_t imu_init(int sda_pin, int scl_pin);

// Reads measurements from the IMU
esp_err_t imu_read(IMUReading *reading);

#endif
