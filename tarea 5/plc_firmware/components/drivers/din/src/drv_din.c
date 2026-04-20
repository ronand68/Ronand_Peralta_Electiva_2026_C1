#include "drv_din.h"

esp_err_t drv_din_init(const drv_din_cfg_t *cfg)
{
    if (cfg == NULL) {
        return ESP_ERR_INVALID_ARG;
    }
    return ESP_OK;
}

esp_err_t drv_din_update(void)
{
    return ESP_OK;
}

bool drv_din_get(uint8_t channel)
{
    (void)channel;
    return false;
}