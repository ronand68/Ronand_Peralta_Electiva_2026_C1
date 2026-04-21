#include "drv_adc_ext.h"

#include <string.h>

#include "esp_check.h"

static driver_health_t s_health = {0};
static drv_adc_ext_cfg_t s_cfg = {0};
static int32_t s_raw[PLC_MAX_AI_CHANNELS] = {0};

esp_err_t drv_adc_ext_init(const drv_adc_ext_cfg_t *cfg)
{
    ESP_RETURN_ON_FALSE(cfg != NULL, ESP_ERR_INVALID_ARG, "drv_adc_ext", "cfg null");
    memcpy(&s_cfg, cfg, sizeof(s_cfg));
    s_health.initialized = true;
    s_health.status = DRIVER_STATUS_READY;
    s_health.last_error = ESP_OK;
    return ESP_OK;
}

esp_err_t drv_adc_ext_update(void)
{
    ESP_RETURN_ON_FALSE(s_health.initialized, ESP_ERR_INVALID_STATE, "drv_adc_ext", "not init");
    return ESP_OK;
}

int32_t drv_adc_ext_get_raw(uint8_t channel)
{
    if (!s_health.initialized || channel >= s_cfg.channel_count) {
        return 0;
    }
    return s_raw[channel];
}

float drv_adc_ext_get_scaled(uint8_t channel)
{
    if (!s_health.initialized || channel >= s_cfg.channel_count) {
        return 0.0f;
    }
    return ((float)s_raw[channel] * s_cfg.gain[channel]) + s_cfg.offset[channel];
}

esp_err_t drv_adc_ext_mock_set(uint8_t channel, int32_t raw_value)
{
    ESP_RETURN_ON_FALSE(channel < PLC_MAX_AI_CHANNELS, ESP_ERR_INVALID_ARG, "drv_adc_ext", "bad channel");
    s_raw[channel] = raw_value;
    return ESP_OK;
}

driver_health_t drv_adc_ext_get_health(void)
{
    return s_health;
}
