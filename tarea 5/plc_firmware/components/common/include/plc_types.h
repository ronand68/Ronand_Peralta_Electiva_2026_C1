#ifndef PLC_TYPES_H
#define PLC_TYPES_H

#include <stdbool.h>
#include <stdint.h>
#include "esp_err.h"

typedef enum {
    PLC_STATE_BOOT = 0,
    PLC_STATE_INIT,
    PLC_STATE_STOP,
    PLC_STATE_RUN,
    PLC_STATE_MANUAL,
    PLC_STATE_MAINTENANCE,
    PLC_STATE_WARNING,
    PLC_STATE_ERROR
} plc_state_t;

typedef enum {
    DRIVER_STATUS_UNINIT = 0,
    DRIVER_STATUS_READY,
    DRIVER_STATUS_BUSY,
    DRIVER_STATUS_WARN,
    DRIVER_STATUS_ERROR
} driver_status_t;

typedef struct {
    bool initialized;
    driver_status_t status;
    esp_err_t last_error;
} driver_health_t;

#endif