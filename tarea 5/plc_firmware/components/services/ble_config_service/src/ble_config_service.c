#include "ble_config_service.h"

#include <string.h>

#include "esp_check.h"

static ble_config_service_cfg_t s_cfg = {0};
static ble_config_service_status_t s_status = {0};

esp_err_t ble_config_service_init(const ble_config_service_cfg_t *cfg)
{
    ESP_RETURN_ON_FALSE(cfg != NULL, ESP_ERR_INVALID_ARG, "ble_config_service", "cfg null");
    memcpy(&s_cfg, cfg, sizeof(s_cfg));
    s_status.running = false;
    s_status.connected = false;
    s_status.session_count = 0U;
    s_status.last_error = ESP_OK;
    return ESP_OK;
}

esp_err_t ble_config_service_start(void)
{
    s_status.running = true;
    return ESP_OK;
}

esp_err_t ble_config_service_stop(void)
{
    s_status.running = false;
    s_status.connected = false;
    return ESP_OK;
}

esp_err_t ble_config_service_get_status(ble_config_service_status_t *status)
{
    ESP_RETURN_ON_FALSE(status != NULL, ESP_ERR_INVALID_ARG, "ble_config_service", "status null");
    *status = s_status;
    return ESP_OK;
}
