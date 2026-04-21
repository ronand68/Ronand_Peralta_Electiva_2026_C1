#include "drv_lcd.h"

#include <string.h>

#include "esp_check.h"

static driver_health_t s_health = {0};
static drv_lcd_cfg_t s_cfg = {0};
static uint8_t s_backlight = 100;

esp_err_t drv_lcd_init(const drv_lcd_cfg_t *cfg)
{
    ESP_RETURN_ON_FALSE(cfg != NULL, ESP_ERR_INVALID_ARG, "drv_lcd", "cfg null");
    memcpy(&s_cfg, cfg, sizeof(s_cfg));
    s_health.initialized = true;
    s_health.status = DRIVER_STATUS_READY;
    s_health.last_error = ESP_OK;
    return ESP_OK;
}

esp_err_t drv_lcd_clear(uint16_t color)
{
    (void)color;
    ESP_RETURN_ON_FALSE(s_health.initialized, ESP_ERR_INVALID_STATE, "drv_lcd", "not init");
    return ESP_OK;
}

esp_err_t drv_lcd_set_backlight(uint8_t percent)
{
    ESP_RETURN_ON_FALSE(s_health.initialized, ESP_ERR_INVALID_STATE, "drv_lcd", "not init");
    if (percent > 100U) {
        percent = 100U;
    }
    s_backlight = percent;
    return ESP_OK;
}

esp_err_t drv_lcd_draw_pixel(uint16_t x, uint16_t y, uint16_t color)
{
    (void)x;
    (void)y;
    (void)color;
    ESP_RETURN_ON_FALSE(s_health.initialized, ESP_ERR_INVALID_STATE, "drv_lcd", "not init");
    return ESP_OK;
}

esp_err_t drv_lcd_present(void)
{
    ESP_RETURN_ON_FALSE(s_health.initialized, ESP_ERR_INVALID_STATE, "drv_lcd", "not init");
    return ESP_OK;
}

driver_health_t drv_lcd_get_health(void)
{
    return s_health;
}
