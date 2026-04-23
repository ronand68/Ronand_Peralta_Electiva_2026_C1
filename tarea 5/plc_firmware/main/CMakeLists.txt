idf_component_register(
    SRCS "main.c"
    INCLUDE_DIRS "."
    REQUIRES
        common
        bsp
        i2c
        din
        dout
        adc_ext
        energy
        ao_4_20ma
        ao_0_10v
        lcd
        touch
        usb_device
        sdcard
        openplc_bridge
        diac_bridge
        plc_core
        wifi_manager
        web_config_service
        ble_config_service
        lvgl_port
        ui_binding
        ui_views
)
