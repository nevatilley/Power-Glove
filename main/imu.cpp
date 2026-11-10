#include "imu.h"               // Includes IMU functions
#include "esp_adc/adc_oneshot.h"  // Allows the ESP32 to read analog signals
#include "esp_err.h"             // Handles ESP32 errors
#include "esp_log.h"             // Allows the ESP32 to log information
#include "esp_timer.h"           // Allows the ESP32 to use timers

/* The IMU (Inertial Measurement Unit) contains two sensors:
 * 1. Gyroscope (measures angular velocity) (degrees per second)
 * 2. Accelerometer (measures linear acceleration) (meters per second squared)
 */

/* Configuration */

/* Data Type */
// 3 component vector for sensor readings
struct Vector3 {
    float x;  // X-axis component
    float y;  // Y-axis component
    float z;  // Z-axis component
};

// orientation of the board
struct Orientation {
    float roll = 0;     // Rotation around the X-axis (degrees)
    float pitch = 0;    // Rotation around the Y-axis (degrees)
    float yaw = 0;      // Rotation around the Z-axis (degrees) 
};

/* Calibration */

/* Filter*/

/* IMU */