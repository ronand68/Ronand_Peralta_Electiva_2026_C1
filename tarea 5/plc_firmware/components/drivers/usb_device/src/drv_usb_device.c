#include "drv_usb_device.h"

#include <string.h>

#include "esp_check.h"

static drv_usb_device_cfg_t s_cfg = {0};
static drv_usb_device_status_t s_status = {0};
static driver_health_t s_health = {0};

esp_err_t drv_usb_device_init(const drv_usb_device_cfg_t *cfg)
{
    ESP_RETURN_ON_FALSE(cfg != NULL, ESP_ERR_INVALID_ARG, "drv_usb_device", "cfg null");
    memcpy(&s_cfg, cfg, sizeof(s_cfg));
    s_health.initialized = true;
    s_health.status = DRIVER_STATUS_READY;
    s_health.last_error = ESP_OK;
    s_status.started = false;
    s_status.mounted = false;
    s_status.tx_bytes = 0U;
    s_status.rx_bytes = 0U;
    s_status.last_error = ESP_OK;
    return ESP_OK;
}

esp_err_t drv_usb_device_start(void)
{
    s_status.started = true;
    s_status.mounted = true;
    return ESP_OK;
}

esp_err_t drv_usb_device_stop(void)
{
    s_status.started = false;
    s_status.mounted = false;
    return ESP_OK;
}

esp_err_t drv_usb_device_cdc_write(const uint8_t *data, size_t len)
{
    ESP_RETURN_ON_FALSE(data != NULL, ESP_ERR_INVALID_ARG, "drv_usb_device", "data null");
    ESP_RETURN_ON_FALSE(s_status.started, ESP_ERR_INVALID_STATE, "drv_usb_device", "not started");
    s_status.tx_bytes += (uint32_t)len;
    return ESP_OK;
}

esp_err_t drv_usb_device_cdc_read(uint8_t *data, size_t max_len, size_t *out_len)
{
    ESP_RETURN_ON_FALSE(data != NULL, ESP_ERR_INVALID_ARG, "drv_usb_device", "data null");
    ESP_RETURN_ON_FALSE(out_len != NULL, ESP_ERR_INVALID_ARG, "drv_usb_device", "out_len null");
    ESP_RETURN_ON_FALSE(s_status.started, ESP_ERR_INVALID_STATE, "drv_usb_device", "not started");
    (void)max_len;
    *out_len = 0U;
    return ESP_OK;
}

esp_err_t drv_usb_device_get_status(drv_usb_device_status_t *status)
{
    ESP_RETURN_ON_FALSE(status != NULL, ESP_ERR_INVALID_ARG, "drv_usb_device", "status null");
    *status = s_status;
    return ESP_OK;
}

driver_health_t drv_usb_device_get_health(void)
{
    return s_health;
}
