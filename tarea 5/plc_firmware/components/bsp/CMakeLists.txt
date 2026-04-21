idf_component_register(
    SRCS "src/board_io_map.c"
    INCLUDE_DIRS "include"
    REQUIRES esp_driver_gpio common
)
