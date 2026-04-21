idf_component_register(
    SRCS "src/drv_i2c.c"
    INCLUDE_DIRS "include"
    REQUIRES common
    PRIV_REQUIRES esp_driver_i2c
)
