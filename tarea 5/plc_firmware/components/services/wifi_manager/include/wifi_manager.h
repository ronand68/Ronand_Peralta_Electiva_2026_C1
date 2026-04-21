#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <stdbool.h>
#include <stdint.h>

#include "esp_err.h"

typedef enum {
    WIFI_MANAGER_MODE_OFF = 0,
    WIFI_MANAGER_MODE_STA,
    WIFI_MANAGER_MODE_AP,
    WIFI_MANAGER_MODE_APSTA
} wifi_manager_mode_t;

typedef struct {
    wifi_manager_mode_t mode;
    char sta_ssid[32];
    char sta_pass[64];
    char ap_ssid[32];
    char ap_pass[64];
    uint8_t ap_channel;
} wifi_manager_cfg_t;

typedef struct {
    bool started;
    bool sta_connected;
    bool ap_active;
    int8_t rssi;
    esp_err_t last_error;
} wifi_manager_status_t;

esp_err_t wifi_manager_init(const wifi_manager_cfg_t *cfg);
esp_err_t wifi_manager_start(void);
esp_err_t wifi_manager_stop(void);
esp_err_t wifi_manager_get_status(wifi_manager_status_t *status);

#endif
