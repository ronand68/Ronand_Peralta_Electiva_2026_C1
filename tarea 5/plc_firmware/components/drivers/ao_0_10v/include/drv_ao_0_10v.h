#ifndef DRV_AO_0_10V_H
#define DRV_AO_0_10V_H

#include <stdint.h>

#include "esp_err.h"
#include "plc_proto_types.h"
#include "plc_types.h"

typedef struct {
    uint8_t channel_count;
    float min_engineering[PLC_MAX_AO_CHANNELS];
    float max_engineering[PLC_MAX_AO_CHANNELS];
} drv_ao_0_10v_cfg_t;

esp_err_t drv_ao_0_10v_init(const drv_ao_0_10v_cfg_t *cfg);
esp_err_t drv_ao_0_10v_set_voltage(uint8_t channel, float voltage);
esp_err_t drv_ao_0_10v_set_engineering(uint8_t channel, float value);
float drv_ao_0_10v_get_voltage(uint8_t channel);
driver_health_t drv_ao_0_10v_get_health(void);

#endif
