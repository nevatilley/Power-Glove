
idf_component_register(SRCS "fsr.cpp" "imu.cpp" "flex_sensors.cpp" "main.cpp"
                       PRIV_REQUIRES spi_flash esp_adc esp_driver_i2c
                       INCLUDE_DIRS "")
