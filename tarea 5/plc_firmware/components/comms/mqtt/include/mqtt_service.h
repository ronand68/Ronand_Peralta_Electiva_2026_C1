#ifndef MQTT_SERVICE_H
#define MQTT_SERVICE_H

#include <stdbool.h>
#include <stdint.h>

#include "esp_err.h"
#include "plc_proto_types.h"

typedef struct {
    char broker_uri[128];
    uint16_t port;
    char client_id[64];
    char topic_pub[PLC_MAX_MQTT_TOPIC_LEN];
    char topic_sub[PLC_MAX_MQTT_TOPIC_LEN];
} mqtt_service_cfg_t;

typedef struct {
    bool running;
    bool connected;
    uint32_t pub_count;
    uint32_t sub_count;
    esp_err_t last_error;
} mqtt_service_status_t;

esp_err_t mqtt_service_init(const mqtt_service_cfg_t *cfg);
esp_err_t mqtt_service_start(void);
esp_err_t mqtt_service_stop(void);
esp_err_t mqtt_service_publish(const char *topic, const char *payload);
esp_err_t mqtt_service_mock_receive(const char *topic, const char *payload);
esp_err_t mqtt_service_get_status(mqtt_service_status_t *status);

#endif
