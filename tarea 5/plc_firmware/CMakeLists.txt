# The following five lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.22)

set(EXTRA_COMPONENT_DIRS
    "${CMAKE_CURRENT_LIST_DIR}/components/bsp"
    "${CMAKE_CURRENT_LIST_DIR}/components/common"
    "${CMAKE_CURRENT_LIST_DIR}/components/drivers/i2c"
    "${CMAKE_CURRENT_LIST_DIR}/components/drivers/din"
    "${CMAKE_CURRENT_LIST_DIR}/components/drivers/dout"
    "${CMAKE_CURRENT_LIST_DIR}/components/drivers/touch"
    "${CMAKE_CURRENT_LIST_DIR}/components/drivers/lcd"
    "${CMAKE_CURRENT_LIST_DIR}/components/drivers/adc_ext"
    "${CMAKE_CURRENT_LIST_DIR}/components/drivers/energy"
    "${CMAKE_CURRENT_LIST_DIR}/components/drivers/ao_4_20ma"
    "${CMAKE_CURRENT_LIST_DIR}/components/drivers/ao_0_10v"
    "${CMAKE_CURRENT_LIST_DIR}/components/drivers/usb_device"
    "${CMAKE_CURRENT_LIST_DIR}/components/drivers/sdcard"
    "${CMAKE_CURRENT_LIST_DIR}/components/hmi/lvgl_port"
    "${CMAKE_CURRENT_LIST_DIR}/components/hmi/ui_binding"
    "${CMAKE_CURRENT_LIST_DIR}/components/hmi/ui_views"
    "${CMAKE_CURRENT_LIST_DIR}/components/comms/modbus"
    "${CMAKE_CURRENT_LIST_DIR}/components/comms/mqtt"
    "${CMAKE_CURRENT_LIST_DIR}/components/comms/openplc_bridge"
    "${CMAKE_CURRENT_LIST_DIR}/components/comms/diac_bridge"
    "${CMAKE_CURRENT_LIST_DIR}/components/plc_core"
    "${CMAKE_CURRENT_LIST_DIR}/components/services/wifi_manager"
    "${CMAKE_CURRENT_LIST_DIR}/components/services/web_config_service"
    "${CMAKE_CURRENT_LIST_DIR}/components/services/ble_config_service"
)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)

set(PROJECT_COMPONENT_EXCLUDE_DIRS
    "${CMAKE_CURRENT_LIST_DIR}/components/comms"
    "${CMAKE_CURRENT_LIST_DIR}/components/drivers"
    "${CMAKE_CURRENT_LIST_DIR}/components/hmi"
    "${CMAKE_CURRENT_LIST_DIR}/components/plc_core"
    "${CMAKE_CURRENT_LIST_DIR}/components/services"
)
idf_build_set_property(EXTRA_COMPONENT_EXCLUDE_DIRS "${PROJECT_COMPONENT_EXCLUDE_DIRS}" APPEND)

project(plc_firmware)
