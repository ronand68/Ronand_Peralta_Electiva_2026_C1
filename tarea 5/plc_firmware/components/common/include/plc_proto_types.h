#ifndef PLC_PROTO_TYPES_H
#define PLC_PROTO_TYPES_H

#include <stdbool.h>
#include <stdint.h>

#include "esp_err.h"
#include "plc_types.h"

#define PLC_MAX_AI_CHANNELS        16
#define PLC_MAX_AO_CHANNELS        16
#define PLC_MAX_DI_CHANNELS        32
#define PLC_MAX_DO_CHANNELS        32
#define PLC_MAX_MODBUS_HOLDING     256
#define PLC_MAX_MODBUS_COILS       256
#define PLC_MAX_MQTT_TOPIC_LEN     96
#define PLC_MAX_DEVICE_NAME_LEN    32
#define PLC_MAX_PROC_VARS          64

typedef struct {
    float ai[PLC_MAX_AI_CHANNELS];
    float ao[PLC_MAX_AO_CHANNELS];
    uint32_t di_bitmap;
    uint32_t do_bitmap;
    float energy_voltage;
    float energy_current;
    float energy_power;
    float energy_frequency;
} plc_io_snapshot_t;

typedef enum {
    PLC_RUNTIME_STOPPED = 0,
    PLC_RUNTIME_STARTING,
    PLC_RUNTIME_RUNNING,
    PLC_RUNTIME_FAULT
} plc_runtime_state_t;

typedef struct {
    bool valid;
    char name[PLC_MAX_DEVICE_NAME_LEN];
    float value;
} plc_process_var_t;

#endif
