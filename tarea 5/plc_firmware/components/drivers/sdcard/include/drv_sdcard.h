#ifndef DRV_SDCARD_H
#define DRV_SDCARD_H

#include <stdbool.h>
#include <stdint.h>

#include "esp_err.h"
#include "plc_types.h"

typedef enum {
    DRV_SDCARD_MODE_SDSPI = 0,
    DRV_SDCARD_MODE_SDMMC
} drv_sdcard_mode_t;

typedef struct {
    drv_sdcard_mode_t mode;
    char mount_point[32];
    bool format_if_mount_failed;
} drv_sdcard_cfg_t;

typedef struct {
    bool mounted;
    uint64_t total_bytes;
    uint64_t free_bytes;
    esp_err_t last_error;
} drv_sdcard_status_t;

esp_err_t drv_sdcard_init(const drv_sdcard_cfg_t *cfg);
esp_err_t drv_sdcard_mount(void);
esp_err_t drv_sdcard_unmount(void);
esp_err_t drv_sdcard_get_status(drv_sdcard_status_t *status);
driver_health_t drv_sdcard_get_health(void);

#endif
