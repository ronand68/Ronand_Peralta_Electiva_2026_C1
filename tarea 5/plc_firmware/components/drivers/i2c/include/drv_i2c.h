#ifndef DRV_I2C_H
#define DRV_I2C_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

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
esp_err_t drv_i2c_deinit(void);
esp_err_t drv_i2c_write(uint8_t dev_addr,
                        const uint8_t *data,
                        size_t len,
                        uint32_t timeout_ms);
esp_err_t drv_i2c_read(uint8_t dev_addr,
                       uint8_t *data,
                       size_t len,
                       uint32_t timeout_ms);
esp_err_t drv_i2c_write_read(uint8_t dev_addr,
                             const uint8_t *tx_data,
                             size_t tx_len,
                             uint8_t *rx_data,
                             size_t rx_len,
                             uint32_t timeout_ms);
esp_err_t drv_i2c_probe(uint8_t dev_addr, uint32_t timeout_ms);
driver_health_t drv_i2c_get_health(void);

#endif
