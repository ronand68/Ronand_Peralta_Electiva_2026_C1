#include "ui_binding.h"

#include <string.h>

#include "esp_check.h"

static ui_model_t s_model;

static void ui_binding_copy_text(char *dst, size_t dst_size, const char *src)
{
    if (dst_size == 0U) {
        return;
    }

    if (src == NULL) {
        dst[0] = '\0';
        return;
    }

    strncpy(dst, src, dst_size - 1U);
    dst[dst_size - 1U] = '\0';
}

esp_err_t ui_binding_init(void)
{
    memset(&s_model, 0, sizeof(s_model));
    ui_binding_copy_text(s_model.project_name, sizeof(s_model.project_name), "PLC ESP32-S3");
    ui_binding_copy_text(s_model.ip_addr, sizeof(s_model.ip_addr), "TODO_VERIFY");
    ui_binding_copy_text(s_model.time_str, sizeof(s_model.time_str), "00:00:00");
    return ESP_OK;
}

const ui_model_t *ui_binding_get_model(void)
{
    return &s_model;
}

esp_err_t ui_binding_update_snapshot(const plc_io_snapshot_t *snapshot)
{
    ESP_RETURN_ON_FALSE(snapshot != NULL, ESP_ERR_INVALID_ARG, "ui_binding", "snapshot null");
    s_model.io = *snapshot;
    return ESP_OK;
}

esp_err_t ui_binding_set_runtime_text(const char *project_name, const char *ip_addr, const char *time_str)
{
    ui_binding_copy_text(s_model.project_name, sizeof(s_model.project_name), project_name);
    ui_binding_copy_text(s_model.ip_addr, sizeof(s_model.ip_addr), ip_addr);
    ui_binding_copy_text(s_model.time_str, sizeof(s_model.time_str), time_str);
    return ESP_OK;
}

esp_err_t ui_binding_set_comm_status(bool wifi_ok, bool web_ok, bool modbus_ok, bool mqtt_ok)
{
    s_model.wifi_ok = wifi_ok;
    s_model.web_ok = web_ok;
    s_model.modbus_ok = modbus_ok;
    s_model.mqtt_ok = mqtt_ok;
    return ESP_OK;
}

esp_err_t ui_binding_set_alarm_count(uint16_t alarm_count)
{
    s_model.alarm_count = alarm_count;
    return ESP_OK;
}
