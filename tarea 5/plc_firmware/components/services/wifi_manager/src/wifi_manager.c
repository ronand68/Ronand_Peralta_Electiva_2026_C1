#include "wifi_manager.h"

#include <string.h>

#include "esp_check.h"

static wifi_manager_cfg_t s_cfg = {0};
static wifi_manager_status_t s_status = {0};

esp_err_t wifi_manager_init(const wifi_manager_cfg_t *cfg)
{
    ESP_RETURN_ON_FALSE(cfg != NULL, ESP_ERR_INVALID_ARG, "wifi_manager", "cfg null");
    memcpy(&s_cfg, cfg, sizeof(s_cfg));
    s_status.started = false;
    s_status.sta_connected = false;
    s_status.ap_active = false;
    s_status.rssi = -127;
    s_status.last_error = ESP_OK;
    return ESP_OK;
}

esp_err_t wifi_manager_start(void)
{
    s_status.started = true;
    s_status.sta_connected = (s_cfg.mode == WIFI_MANAGER_MODE_STA || s_cfg.mode == WIFI_MANAGER_MODE_APSTA);
    s_status.ap_active = (s_cfg.mode == WIFI_MANAGER_MODE_AP || s_cfg.mode == WIFI_MANAGER_MODE_APSTA);
    s_status.rssi = -45;
    return ESP_OK;
}

esp_err_t wifi_manager_stop(void)
{
    s_status.started = false;
    s_status.sta_connected = false;
    s_status.ap_active = false;
    return ESP_OK;
}

esp_err_t wifi_manager_get_status(wifi_manager_status_t *status)
{
    ESP_RETURN_ON_FALSE(status != NULL, ESP_ERR_INVALID_ARG, "wifi_manager", "status null");
    *status = s_status;
    return ESP_OK;
}
