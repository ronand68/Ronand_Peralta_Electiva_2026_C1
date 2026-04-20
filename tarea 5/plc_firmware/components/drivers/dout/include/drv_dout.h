#ifndef DRV_DOUT_H
#define DRV_DOUT_H

#include <stdbool.h>
#include <stdint.h>
#include "esp_err.h"

#define DRV_DOUT_MAX_CHANNELS 16

typedef struct {
    uint8_t channel_count;
    int gpio_list[DRV_DOUT_MAX_CHANNELS];
    bool invert[DRV_DOUT_MAX_CHANNELS];
    bool default_state[DRV_DOUT_MAX_CHANNELS];
} drv_dout_cfg_t;

esp_err_t drv_dout_init(const drv_dout_cfg_t *cfg);
esp_err_t drv_dout_set(uint8_t channel, bool value);
bool drv_dout_get(uint8_t channel);

#endif