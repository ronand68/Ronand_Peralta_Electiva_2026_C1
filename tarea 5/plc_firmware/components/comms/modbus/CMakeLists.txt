idf_component_register(
    SRCS
        "src/modbus_service.c"
        "src/modbus_register_map.c"
    INCLUDE_DIRS "include"
    REQUIRES common
)
