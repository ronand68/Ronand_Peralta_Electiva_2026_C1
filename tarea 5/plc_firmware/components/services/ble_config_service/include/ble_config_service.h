#ifndef BLE_CONFIG_SERVICE_H
#define BLE_CONFIG_SERVICE_H

#include <stdbool.h>
#include <stdint.h>

#include "esp_err.h"

typedef struct {
    char device_name[32];
    bool enable_provisioning;
} ble_config_service_cfg_t;

typedef struct {
    bool running;
    bool connected;
    uint32_t session_count;
    esp_err_t last_error;
} ble_config_service_status_t;

esp_err_t ble_config_service_init(const ble_config_service_cfg_t *cfg);
esp_err_t ble_config_service_start(void);
esp_err_t ble_config_service_stop(void);
esp_err_t ble_config_service_get_status(ble_config_service_status_t *status);

#endif
