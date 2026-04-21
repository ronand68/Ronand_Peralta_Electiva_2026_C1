#include "mqtt_service.h"

#include <string.h>

#include "esp_check.h"

static mqtt_service_cfg_t s_cfg = {0};
static mqtt_service_status_t s_status = {0};

esp_err_t mqtt_service_init(const mqtt_service_cfg_t *cfg)
{
    ESP_RETURN_ON_FALSE(cfg != NULL, ESP_ERR_INVALID_ARG, "mqtt_service", "cfg null");
    memcpy(&s_cfg, cfg, sizeof(s_cfg));
    s_status.running = false;
    s_status.connected = false;
    s_status.pub_count = 0;
    s_status.sub_count = 0;
    s_status.last_error = ESP_OK;
    return ESP_OK;
}

esp_err_t mqtt_service_start(void)
{
    s_status.running = true;
    s_status.connected = true;
    return ESP_OK;
}

esp_err_t mqtt_service_stop(void)
{
    s_status.running = false;
    s_status.connected = false;
    return ESP_OK;
}

esp_err_t mqtt_service_publish(const char *topic, const char *payload)
{
    ESP_RETURN_ON_FALSE(topic != NULL, ESP_ERR_INVALID_ARG, "mqtt_service", "topic null");
    ESP_RETURN_ON_FALSE(payload != NULL, ESP_ERR_INVALID_ARG, "mqtt_service", "payload null");
    ESP_RETURN_ON_FALSE(s_status.running, ESP_ERR_INVALID_STATE, "mqtt_service", "not running");
    s_status.pub_count++;
    return ESP_OK;
}

esp_err_t mqtt_service_mock_receive(const char *topic, const char *payload)
{
    ESP_RETURN_ON_FALSE(topic != NULL, ESP_ERR_INVALID_ARG, "mqtt_service", "topic null");
    ESP_RETURN_ON_FALSE(payload != NULL, ESP_ERR_INVALID_ARG, "mqtt_service", "payload null");
    s_status.sub_count++;
    return ESP_OK;
}

esp_err_t mqtt_service_get_status(mqtt_service_status_t *status)
{
    ESP_RETURN_ON_FALSE(status != NULL, ESP_ERR_INVALID_ARG, "mqtt_service", "status null");
    *status = s_status;
    return ESP_OK;
}
