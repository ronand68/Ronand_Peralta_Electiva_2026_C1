#include "drv_ao_0_10v.h"

#include <string.h>

#include "esp_check.h"

static driver_health_t s_health = {0};
static drv_ao_0_10v_cfg_t s_cfg = {0};
static float s_voltage[PLC_MAX_AO_CHANNELS] = {0};

static float scale_to_voltage(const drv_ao_0_10v_cfg_t *cfg, uint8_t channel, float value)
{
    float min = cfg->min_engineering[channel];
    float max = cfg->max_engineering[channel];
    float ratio;

    if (max <= min) {
        return 0.0f;
    }

    ratio = (value - min) / (max - min);
    if (ratio < 0.0f) {
        ratio = 0.0f;
    }
    if (ratio > 1.0f) {
        ratio = 1.0f;
    }
    return ratio * 10.0f;
}

esp_err_t drv_ao_0_10v_init(const drv_ao_0_10v_cfg_t *cfg)
{
    ESP_RETURN_ON_FALSE(cfg != NULL, ESP_ERR_INVALID_ARG, "drv_ao_0_10v", "cfg null");
    memcpy(&s_cfg, cfg, sizeof(s_cfg));
    s_health.initialized = true;
    s_health.status = DRIVER_STATUS_READY;
    s_health.last_error = ESP_OK;
    return ESP_OK;
}

esp_err_t drv_ao_0_10v_set_voltage(uint8_t channel, float voltage)
{
    ESP_RETURN_ON_FALSE(s_health.initialized, ESP_ERR_INVALID_STATE, "drv_ao_0_10v", "not init");
    ESP_RETURN_ON_FALSE(channel < s_cfg.channel_count, ESP_ERR_INVALID_ARG, "drv_ao_0_10v", "bad channel");

    if (voltage < 0.0f) {
        voltage = 0.0f;
    }
    if (voltage > 10.0f) {
        voltage = 10.0f;
    }
    s_voltage[channel] = voltage;
    return ESP_OK;
}

esp_err_t drv_ao_0_10v_set_engineering(uint8_t channel, float value)
{
    ESP_RETURN_ON_FALSE(channel < s_cfg.channel_count, ESP_ERR_INVALID_ARG, "drv_ao_0_10v", "bad channel");
    return drv_ao_0_10v_set_voltage(channel, scale_to_voltage(&s_cfg, channel, value));
}

float drv_ao_0_10v_get_voltage(uint8_t channel)
{
    if (!s_health.initialized || channel >= s_cfg.channel_count) {
        return 0.0f;
    }
    return s_voltage[channel];
}

driver_health_t drv_ao_0_10v_get_health(void)
{
    return s_health;
}
