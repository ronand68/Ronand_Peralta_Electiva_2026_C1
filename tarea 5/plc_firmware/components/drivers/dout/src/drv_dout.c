#include "drv_dout.h"

esp_err_t drv_dout_init(const drv_dout_cfg_t *cfg)
{
    if (cfg == NULL) {
        return ESP_ERR_INVALID_ARG;
    }
    return ESP_OK;
}

esp_err_t drv_dout_set(uint8_t channel, bool value)
{
    (void)channel;
    (void)value;
    return ESP_OK;
}

bool drv_dout_get(uint8_t channel)
{
    (void)channel;
    return false;
}