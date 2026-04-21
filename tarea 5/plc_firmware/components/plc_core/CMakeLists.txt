idf_component_register(
    SRCS "src/plc_core.c"
    INCLUDE_DIRS "include"
    REQUIRES
        common
        adc_ext
        din
        dout
        energy
        ao_4_20ma
        ao_0_10v
        openplc_bridge
        diac_bridge
)
