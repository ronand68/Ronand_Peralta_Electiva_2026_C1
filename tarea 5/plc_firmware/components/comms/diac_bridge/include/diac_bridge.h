#ifndef DIAC_BRIDGE_H
#define DIAC_BRIDGE_H

#include <stdint.h>

#include "esp_err.h"
#include "plc_proto_types.h"

typedef struct {
    plc_runtime_state_t runtime_state;
    uint32_t event_count;
    esp_err_t last_error;
} diac_bridge_status_t;

esp_err_t diac_bridge_init(void);
esp_err_t diac_bridge_start(void);
esp_err_t diac_bridge_stop(void);
esp_err_t diac_bridge_push_io(const plc_io_snapshot_t *snapshot);
esp_err_t diac_bridge_get_status(diac_bridge_status_t *status);

#endif
