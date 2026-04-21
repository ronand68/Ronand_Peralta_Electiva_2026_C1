#include "diac_bridge.h"

#include <string.h>

#include "esp_check.h"

static diac_bridge_status_t s_status = {
    .runtime_state = PLC_RUNTIME_STOPPED,
    .event_count = 0,
    .last_error = ESP_OK,
};
static plc_io_snapshot_t s_last_io = {0};

esp_err_t diac_bridge_init(void)
{
    memset(&s_last_io, 0, sizeof(s_last_io));
    s_status.runtime_state = PLC_RUNTIME_STOPPED;
    s_status.event_count = 0;
    s_status.last_error = ESP_OK;
    return ESP_OK;
}

esp_err_t diac_bridge_start(void)
{
    s_status.runtime_state = PLC_RUNTIME_RUNNING;
    return ESP_OK;
}

esp_err_t diac_bridge_stop(void)
{
    s_status.runtime_state = PLC_RUNTIME_STOPPED;
    return ESP_OK;
}

esp_err_t diac_bridge_push_io(const plc_io_snapshot_t *snapshot)
{
    ESP_RETURN_ON_FALSE(snapshot != NULL, ESP_ERR_INVALID_ARG, "diac_bridge", "snapshot null");
    s_last_io = *snapshot;
    s_status.event_count++;
    return ESP_OK;
}

esp_err_t diac_bridge_get_status(diac_bridge_status_t *status)
{
    ESP_RETURN_ON_FALSE(status != NULL, ESP_ERR_INVALID_ARG, "diac_bridge", "status null");
    *status = s_status;
    return ESP_OK;
}
