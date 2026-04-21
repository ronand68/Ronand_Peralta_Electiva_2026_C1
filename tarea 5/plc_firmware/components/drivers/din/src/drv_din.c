#include "drv_din.h"

#include <string.h>

#include "driver/gpio.h"
#include "esp_check.h"
#include "esp_log.h"

static const char *TAG = "drv_din";

static drv_din_cfg_t s_cfg;
static uint32_t s_bitmap = 0U;
static bool s_initialized = false;

esp_err_t drv_din_init(const drv_din_cfg_t *cfg)
{
    ESP_RETURN_ON_FALSE(cfg != NULL, ESP_ERR_INVALID_ARG, TAG, "cfg es NULL");
    ESP_RETURN_ON_FALSE(cfg->channel_count > 0U && cfg->channel_count <= DRV_DIN_MAX_CHANNELS,
                        ESP_ERR_INVALID_ARG, TAG, "channel_count invalido");

    memset(&s_cfg, 0, sizeof(s_cfg));
    memcpy(&s_cfg, cfg, sizeof(s_cfg));

    for (uint8_t i = 0; i < s_cfg.channel_count; i++) {
        gpio_config_t io_cfg = {
            .pin_bit_mask = (1ULL << s_cfg.gpio_list[i]),
            .mode = GPIO_MODE_INPUT,
            .pull_up_en = GPIO_PULLUP_DISABLE,
            .pull_down_en = GPIO_PULLDOWN_DISABLE,
            .intr_type = GPIO_INTR_DISABLE,
        };
        ESP_RETURN_ON_ERROR(gpio_config(&io_cfg), TAG, "Error configurando GPIO de entrada");
    }

    s_bitmap = 0U;
    s_initialized = true;

    ESP_LOGI(TAG, "DIN listo con %u canales", s_cfg.channel_count);
    return ESP_OK;
}

esp_err_t drv_din_update(void)
{
    ESP_RETURN_ON_FALSE(s_initialized, ESP_ERR_INVALID_STATE, TAG, "DIN no inicializado");

    uint32_t bitmap = 0U;

    for (uint8_t i = 0; i < s_cfg.channel_count; i++) {
        int level = gpio_get_level(s_cfg.gpio_list[i]);
        if (s_cfg.invert[i]) {
            level = !level;
        }

        if (level) {
            bitmap |= (1UL << i);
        }
    }

    s_bitmap = bitmap;
    return ESP_OK;
}

bool drv_din_get(uint8_t channel)
{
    if (!s_initialized || channel >= s_cfg.channel_count) {
        return false;
    }

    return ((s_bitmap >> channel) & 0x01U) != 0U;
}

uint32_t drv_din_get_bitmap(void)
{
    return s_bitmap;
}
