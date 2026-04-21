idf_component_register(
    SRCS "src/drv_dout.c"
    INCLUDE_DIRS "include"
    REQUIRES common
    PRIV_REQUIRES esp_driver_gpio
)
