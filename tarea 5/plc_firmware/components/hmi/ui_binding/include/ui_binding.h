#ifndef UI_BINDING_H
#define UI_BINDING_H

#include <stdbool.h>
#include <stdint.h>

#include "esp_err.h"
#include "plc_proto_types.h"

typedef struct {
    plc_io_snapshot_t io;
    char project_name[32];
    char ip_addr[32];
    char time_str[16];
    uint16_t alarm_count;
    bool modbus_ok;
    bool mqtt_ok;
    bool web_ok;
    bool wifi_ok;
} ui_model_t;

esp_err_t ui_binding_init(void);
const ui_model_t *ui_binding_get_model(void);
esp_err_t ui_binding_update_snapshot(const plc_io_snapshot_t *snapshot);
esp_err_t ui_binding_set_runtime_text(const char *project_name, const char *ip_addr, const char *time_str);
esp_err_t ui_binding_set_comm_status(bool wifi_ok, bool web_ok, bool modbus_ok, bool mqtt_ok);
esp_err_t ui_binding_set_alarm_count(uint16_t alarm_count);

#endif
