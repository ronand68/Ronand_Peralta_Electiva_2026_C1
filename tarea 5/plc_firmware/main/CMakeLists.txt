idf_component_register(
    SRCS "main.c"
    INCLUDE_DIRS "."
    REQUIRES common bsp i2c din dout
)