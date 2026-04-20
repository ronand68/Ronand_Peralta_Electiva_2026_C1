#ifndef DRV_I2C_H
#define DRV_I2C_H

#include <stdint.h>
#include <stdbool.h>
#include "esp_err.h"
#include "plc_types.h"

typedef struct {
    int port;
    int gpio_sda;
    int gpio_scl;
    uint32_t freq_hz;
    bool pullup_en;
} drv_i2c_cfg_t;

esp_err_t drv_i2c_init(const drv_i2c_cfg_t *cfg);
driver_health_t drv_i2c_get_health(void);

#endif