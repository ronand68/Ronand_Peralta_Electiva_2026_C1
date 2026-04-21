#include "drv_sdcard.h"

#include <string.h>

#include "esp_check.h"

static drv_sdcard_cfg_t s_cfg = {0};
static drv_sdcard_status_t s_status = {0};
static driver_health_t s_health = {0};

esp_err_t drv_sdcard_init(const drv_sdcard_cfg_t *cfg)
{
    ESP_RETURN_ON_FALSE(cfg != NULL, ESP_ERR_INVALID_ARG, "drv_sdcard", "cfg null");
    memcpy(&s_cfg, cfg, sizeof(s_cfg));
    s_health.initialized = true;
    s_health.status = DRIVER_STATUS_READY;
    s_health.last_error = ESP_OK;
    s_status.mounted = false;
    s_status.total_bytes = 0U;
    s_status.free_bytes = 0U;
    s_status.last_error = ESP_OK;
    return ESP_OK;
}

esp_err_t drv_sdcard_mount(void)
{
    ESP_RETURN_ON_FALSE(s_health.initialized, ESP_ERR_INVALID_STATE, "drv_sdcard", "not init");
    s_status.mounted = true;
    s_status.total_bytes = 32ULL * 1024ULL * 1024ULL;
    s_status.free_bytes = 28ULL * 1024ULL * 1024ULL;
    return ESP_OK;
}

esp_err_t drv_sdcard_unmount(void)
{
    s_status.mounted = false;
    return ESP_OK;
}

esp_err_t drv_sdcard_get_status(drv_sdcard_status_t *status)
{
    ESP_RETURN_ON_FALSE(status != NULL, ESP_ERR_INVALID_ARG, "drv_sdcard", "status null");
    *status = s_status;
    return ESP_OK;
}

driver_health_t drv_sdcard_get_health(void)
{
    return s_health;
}
