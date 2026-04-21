#include "drv_energy.h"

#include <string.h>

#include "esp_check.h"

static driver_health_t s_health = {0};
static drv_energy_data_t s_data = {0};

esp_err_t drv_energy_init(const drv_energy_cfg_t *cfg)
{
    ESP_RETURN_ON_FALSE(cfg != NULL, ESP_ERR_INVALID_ARG, "drv_energy", "cfg null");
    s_health.initialized = true;
    s_health.status = DRIVER_STATUS_READY;
    s_health.last_error = ESP_OK;
    return ESP_OK;
}

esp_err_t drv_energy_update(void)
{
    ESP_RETURN_ON_FALSE(s_health.initialized, ESP_ERR_INVALID_STATE, "drv_energy", "not init");
    return ESP_OK;
}

esp_err_t drv_energy_get_data(drv_energy_data_t *data)
{
    ESP_RETURN_ON_FALSE(data != NULL, ESP_ERR_INVALID_ARG, "drv_energy", "data null");
    ESP_RETURN_ON_FALSE(s_health.initialized, ESP_ERR_INVALID_STATE, "drv_energy", "not init");
    *data = s_data;
    return ESP_OK;
}

esp_err_t drv_energy_mock_set(const drv_energy_data_t *data)
{
    ESP_RETURN_ON_FALSE(data != NULL, ESP_ERR_INVALID_ARG, "drv_energy", "data null");
    s_data = *data;
    return ESP_OK;
}

driver_health_t drv_energy_get_health(void)
{
    return s_health;
}
