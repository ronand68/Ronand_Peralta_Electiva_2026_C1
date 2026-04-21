#ifndef DRV_AO_4_20MA_H
#define DRV_AO_4_20MA_H

#include <stdbool.h>
#include <stdint.h>

#include "esp_err.h"
#include "plc_proto_types.h"
#include "plc_types.h"

typedef struct {
    uint8_t channel_count;
    float min_engineering[PLC_MAX_AO_CHANNELS];
    float max_engineering[PLC_MAX_AO_CHANNELS];
} drv_ao_4_20ma_cfg_t;

esp_err_t drv_ao_4_20ma_init(const drv_ao_4_20ma_cfg_t *cfg);
esp_err_t drv_ao_4_20ma_set_current(uint8_t channel, float current_ma);
esp_err_t drv_ao_4_20ma_set_engineering(uint8_t channel, float value);
float drv_ao_4_20ma_get_current(uint8_t channel);
driver_health_t drv_ao_4_20ma_get_health(void);

#endif
