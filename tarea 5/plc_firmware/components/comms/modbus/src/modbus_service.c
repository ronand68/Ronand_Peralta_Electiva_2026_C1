#include "modbus_service.h"

#include <string.h>

#include "esp_check.h"

static modbus_service_cfg_t s_cfg = {0};
static modbus_service_status_t s_status = {0};
static uint16_t s_holding[PLC_MAX_MODBUS_HOLDING] = {0};
static bool s_coils[PLC_MAX_MODBUS_COILS] = {0};

static void modbus_service_init_register_map(void)
{
    memset(s_holding, 0, sizeof(s_holding));
    memset(s_coils, 0, sizeof(s_coils));

    s_coils[MB_COIL_RUN_CMD] = false;
    s_coils[MB_COIL_STOP_CMD] = false;

    s_holding[MB_HREG_AO0_SETPOINT] = 0U;
    s_holding[MB_HREG_AO1_SETPOINT] = 0U;
    s_holding[MB_HREG_SYSTEM_MODE] = 0U;
    s_holding[MB_HREG_IP_OCTET_1] = 0U;
    s_holding[MB_HREG_IP_OCTET_2] = 0U;
    s_holding[MB_HREG_IP_OCTET_3] = 0U;
    s_holding[MB_HREG_IP_OCTET_4] = 0U;
    s_holding[MB_HREG_MQTT_PORT] = 1883U;
}

esp_err_t modbus_service_init(const modbus_service_cfg_t *cfg)
{
    ESP_RETURN_ON_FALSE(cfg != NULL, ESP_ERR_INVALID_ARG, "modbus_service", "cfg null");
    memcpy(&s_cfg, cfg, sizeof(s_cfg));
    modbus_service_init_register_map();
    s_status.running = false;
    s_status.rx_count = 0;
    s_status.tx_count = 0;
    s_status.last_error = ESP_OK;
    return ESP_OK;
}

esp_err_t modbus_service_start(void)
{
    s_status.running = true;
    return ESP_OK;
}

esp_err_t modbus_service_stop(void)
{
    s_status.running = false;
    return ESP_OK;
}

esp_err_t modbus_service_set_holding(uint16_t reg, uint16_t value)
{
    ESP_RETURN_ON_FALSE(reg < PLC_MAX_MODBUS_HOLDING, ESP_ERR_INVALID_ARG, "modbus_service", "bad reg");
    s_holding[reg] = value;
    return ESP_OK;
}

esp_err_t modbus_service_get_holding(uint16_t reg, uint16_t *value)
{
    ESP_RETURN_ON_FALSE(value != NULL, ESP_ERR_INVALID_ARG, "modbus_service", "value null");
    ESP_RETURN_ON_FALSE(reg < PLC_MAX_MODBUS_HOLDING, ESP_ERR_INVALID_ARG, "modbus_service", "bad reg");
    *value = s_holding[reg];
    return ESP_OK;
}

esp_err_t modbus_service_set_coil(uint16_t reg, bool value)
{
    ESP_RETURN_ON_FALSE(reg < PLC_MAX_MODBUS_COILS, ESP_ERR_INVALID_ARG, "modbus_service", "bad reg");
    s_coils[reg] = value;
    return ESP_OK;
}

esp_err_t modbus_service_get_coil(uint16_t reg, bool *value)
{
    ESP_RETURN_ON_FALSE(value != NULL, ESP_ERR_INVALID_ARG, "modbus_service", "value null");
    ESP_RETURN_ON_FALSE(reg < PLC_MAX_MODBUS_COILS, ESP_ERR_INVALID_ARG, "modbus_service", "bad reg");
    *value = s_coils[reg];
    return ESP_OK;
}

esp_err_t modbus_service_get_status(modbus_service_status_t *status)
{
    ESP_RETURN_ON_FALSE(status != NULL, ESP_ERR_INVALID_ARG, "modbus_service", "status null");
    *status = s_status;
    return ESP_OK;
}
