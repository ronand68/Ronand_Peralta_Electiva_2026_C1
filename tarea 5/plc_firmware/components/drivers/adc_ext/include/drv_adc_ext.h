#ifndef DRV_ADC_EXT_H
#define DRV_ADC_EXT_H

#include <stdint.h>

#include "esp_err.h"
#include "plc_proto_types.h"
#include "plc_types.h"

typedef struct {
    uint8_t channel_count;
    float gain[PLC_MAX_AI_CHANNELS];
    float offset[PLC_MAX_AI_CHANNELS];
} drv_adc_ext_cfg_t;

esp_err_t drv_adc_ext_init(const drv_adc_ext_cfg_t *cfg);
esp_err_t drv_adc_ext_update(void);
int32_t drv_adc_ext_get_raw(uint8_t channel);
float drv_adc_ext_get_scaled(uint8_t channel);
esp_err_t drv_adc_ext_mock_set(uint8_t channel, int32_t raw_value);
driver_health_t drv_adc_ext_get_health(void);

#endif
