#include "drv_dout.h"

#include <string.h>

#include "driver/gpio.h"
#include "esp_check.h"
#include "esp_log.h"

static const char *TAG = "drv_dout";

static drv_dout_cfg_t s_cfg;
static uint32_t s_bitmap = 0U;
static bool s_initialized = false;

static int drv_dout_hw_value(uint8_t ch, bool value)
{
    return s_cfg.invert[ch] ? !value : value;
}

esp_err_t drv_dout_init(const drv_dout_cfg_t *cfg)
{
    ESP_RETURN_ON_FALSE(cfg != NULL, ESP_ERR_INVALID_ARG, TAG, "cfg es NULL");
    ESP_RETURN_ON_FALSE(cfg->channel_count > 0U && cfg->channel_count <= DRV_DOUT_MAX_CHANNELS,
                        ESP_ERR_INVALID_ARG, TAG, "channel_count invalido");

    memset(&s_cfg, 0, sizeof(s_cfg));
    memcpy(&s_cfg, cfg, sizeof(s_cfg));

    for (uint8_t i = 0; i < s_cfg.channel_count; i++) {
        gpio_config_t io_cfg = {
            .pin_bit_mask = (1ULL << s_cfg.gpio_list[i]),
            .mode = GPIO_MODE_OUTPUT,
            .pull_up_en = GPIO_PULLUP_DISABLE,
            .pull_down_en = GPIO_PULLDOWN_DISABLE,
            .intr_type = GPIO_INTR_DISABLE,
        };
        ESP_RETURN_ON_ERROR(gpio_config(&io_cfg), TAG, "Error configurando GPIO de salida");

        int hw_value = drv_dout_hw_value(i, s_cfg.default_state[i]);
        ESP_RETURN_ON_ERROR(gpio_set_level(s_cfg.gpio_list[i], hw_value), TAG, "Error aplicando estado inicial");

        if (s_cfg.default_state[i]) {
            s_bitmap |= (1UL << i);
        }
    }

    s_initialized = true;
    ESP_LOGI(TAG, "DOUT listo con %u canales", s_cfg.channel_count);
    return ESP_OK;
}

esp_err_t drv_dout_set(uint8_t channel, bool value)
{
    ESP_RETURN_ON_FALSE(s_initialized, ESP_ERR_INVALID_STATE, TAG, "DOUT no inicializado");
    ESP_RETURN_ON_FALSE(channel < s_cfg.channel_count, ESP_ERR_INVALID_ARG, TAG, "channel invalido");

    int hw_value = drv_dout_hw_value(channel, value);
    ESP_RETURN_ON_ERROR(gpio_set_level(s_cfg.gpio_list[channel], hw_value), TAG, "Error escribiendo salida");

    if (value) {
        s_bitmap |= (1UL << channel);
    } else {
        s_bitmap &= ~(1UL << channel);
    }

    return ESP_OK;
}

bool drv_dout_get(uint8_t channel)
{
    if (!s_initialized || channel >= s_cfg.channel_count) {
        return false;
    }

    return ((s_bitmap >> channel) & 0x01U) != 0U;
}

uint32_t drv_dout_get_bitmap(void)
{
    return s_bitmap;
}

esp_err_t drv_dout_write_bitmap(uint32_t bitmap)
{
    ESP_RETURN_ON_FALSE(s_initialized, ESP_ERR_INVALID_STATE, TAG, "DOUT no inicializado");

    for (uint8_t i = 0; i < s_cfg.channel_count; i++) {
        bool value = ((bitmap >> i) & 0x01U) != 0U;
        ESP_RETURN_ON_ERROR(drv_dout_set(i, value), TAG, "Fallo escribiendo bitmap");
    }

    return ESP_OK;
}
