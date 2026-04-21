#include "drv_touch.h"

#include <string.h>

#include "esp_check.h"

static driver_health_t s_health = {0};
static drv_touch_cfg_t s_cfg = {0};
static drv_touch_point_t s_point = {0};

esp_err_t drv_touch_init(const drv_touch_cfg_t *cfg)
{
    ESP_RETURN_ON_FALSE(cfg != NULL, ESP_ERR_INVALID_ARG, "drv_touch", "cfg null");
    memcpy(&s_cfg, cfg, sizeof(s_cfg));
    s_health.initialized = true;
    s_health.status = DRIVER_STATUS_READY;
    s_health.last_error = ESP_OK;
    return ESP_OK;
}

esp_err_t drv_touch_update(void)
{
    ESP_RETURN_ON_FALSE(s_health.initialized, ESP_ERR_INVALID_STATE, "drv_touch", "not init");
    return ESP_OK;
}

esp_err_t drv_touch_get_point(drv_touch_point_t *point)
{
    ESP_RETURN_ON_FALSE(point != NULL, ESP_ERR_INVALID_ARG, "drv_touch", "point null");
    ESP_RETURN_ON_FALSE(s_health.initialized, ESP_ERR_INVALID_STATE, "drv_touch", "not init");
    *point = s_point;
    return ESP_OK;
}

esp_err_t drv_touch_mock_set(bool pressed, uint16_t x, uint16_t y)
{
    s_point.pressed = pressed;
    s_point.x = x;
    s_point.y = y;
    return ESP_OK;
}

driver_health_t drv_touch_get_health(void)
{
    return s_health;
}
