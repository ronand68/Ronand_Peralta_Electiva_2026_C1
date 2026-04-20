#include "esp_log.h"
#include "board_io_map.h"
#include "drv_i2c.h"

static const char *TAG = "PLC_MAIN";

void app_main(void)
{
    drv_i2c_cfg_t i2c_cfg = {
        .port = BOARD_I2C_PORT,
        .gpio_sda = BOARD_I2C_SDA_GPIO,
        .gpio_scl = BOARD_I2C_SCL_GPIO,
        .freq_hz = BOARD_I2C_FREQ_HZ,
        .pullup_en = true
    };

    ESP_ERROR_CHECK(drv_i2c_init(&i2c_cfg));

    ESP_LOGI(TAG, "Proyecto PLC iniciado");
}