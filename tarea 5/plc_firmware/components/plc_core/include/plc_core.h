#ifndef PLC_CORE_H
#define PLC_CORE_H

#include "esp_err.h"
#include "plc_proto_types.h"

esp_err_t plc_core_init(void);
esp_err_t plc_core_update(void);
esp_err_t plc_core_get_snapshot(plc_io_snapshot_t *snapshot);

#endif
