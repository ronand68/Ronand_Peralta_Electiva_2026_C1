#ifndef MODBUS_SERVICE_H
#define MODBUS_SERVICE_H

#include <stdbool.h>
#include <stdint.h>

#include "esp_err.h"
#include "modbus_register_map.h"
#include "plc_proto_types.h"

typedef enum {
    MODBUS_MODE_DISABLED = 0,
    MODBUS_MODE_RTU_SLAVE,
    MODBUS_MODE_TCP_SERVER
} modbus_mode_t;

typedef struct {
    modbus_mode_t mode;
    uint8_t slave_id;
    uint32_t baudrate;
    uint16_t tcp_port;
} modbus_service_cfg_t;

typedef struct {
    bool running;
    uint32_t rx_count;
    uint32_t tx_count;
    esp_err_t last_error;
} modbus_service_status_t;

esp_err_t modbus_service_init(const modbus_service_cfg_t *cfg);
esp_err_t modbus_service_start(void);
esp_err_t modbus_service_stop(void);
esp_err_t modbus_service_set_holding(uint16_t reg, uint16_t value);
esp_err_t modbus_service_get_holding(uint16_t reg, uint16_t *value);
esp_err_t modbus_service_set_coil(uint16_t reg, bool value);
esp_err_t modbus_service_get_coil(uint16_t reg, bool *value);
esp_err_t modbus_service_get_status(modbus_service_status_t *status);

#endif
