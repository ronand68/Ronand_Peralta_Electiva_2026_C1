#ifndef DRV_ENERGY_H
#define DRV_ENERGY_H

#include "esp_err.h"
#include "plc_types.h"

typedef struct {
    float voltage;
    float current;
    float power;
    float frequency;
    float pf;
    float energy_kwh;
} drv_energy_data_t;

typedef struct {
    uint32_t refresh_ms;
} drv_energy_cfg_t;

esp_err_t drv_energy_init(const drv_energy_cfg_t *cfg);
esp_err_t drv_energy_update(void);
esp_err_t drv_energy_get_data(drv_energy_data_t *data);
esp_err_t drv_energy_mock_set(const drv_energy_data_t *data);
driver_health_t drv_energy_get_health(void);

#endif
