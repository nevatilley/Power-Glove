
idf_component_register(SRCS "fsr.cpp" "imu.cpp" "flex_sensors.cpp" "main.cpp"
                       PRIV_REQUIRES spi_flash esp_adc
                       INCLUDE_DIRS "")
