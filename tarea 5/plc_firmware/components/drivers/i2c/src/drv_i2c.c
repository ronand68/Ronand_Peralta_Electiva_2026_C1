#include "drv_i2c.h"
#include "driver/i2c_master.h"
#include <string.h>

static driver_health_t s_health = {0};

esp_err_t drv_i2c_init(const drv_i2c_cfg_t *cfg)
{
    if (cfg == NULL) {
        return ESP_ERR_INVALID_ARG;
    }

    s_health.initialized = true;
    s_health.status = DRIVER_STATUS_READY;
    s_health.last_error = ESP_OK;

    return ESP_OK;
}

driver_health_t drv_i2c_get_health(void)
{
    return s_health;
}