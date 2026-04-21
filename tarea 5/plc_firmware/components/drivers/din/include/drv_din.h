#ifndef DRV_DIN_H
#define DRV_DIN_H

#include <stdbool.h>
#include <stdint.h>
#include "esp_err.h"

#define DRV_DIN_MAX_CHANNELS 16

typedef struct {
    uint8_t channel_count;
    int gpio_list[DRV_DIN_MAX_CHANNELS];
    bool invert[DRV_DIN_MAX_CHANNELS];
} drv_din_cfg_t;

esp_err_t drv_din_init(const drv_din_cfg_t *cfg);
esp_err_t drv_din_update(void);
bool drv_din_get(uint8_t channel);
uint32_t drv_din_get_bitmap(void);

#endif
