#ifndef WEB_CONFIG_SERVICE_H
#define WEB_CONFIG_SERVICE_H

#include <stdbool.h>
#include <stdint.h>

#include "esp_err.h"

typedef struct {
    uint16_t port;
    bool enable_auth;
} web_config_service_cfg_t;

typedef struct {
    bool running;
    uint32_t request_count;
    esp_err_t last_error;
} web_config_service_status_t;

esp_err_t web_config_service_init(const web_config_service_cfg_t *cfg);
esp_err_t web_config_service_start(void);
esp_err_t web_config_service_stop(void);
esp_err_t web_config_service_get_status(web_config_service_status_t *status);

#endif
