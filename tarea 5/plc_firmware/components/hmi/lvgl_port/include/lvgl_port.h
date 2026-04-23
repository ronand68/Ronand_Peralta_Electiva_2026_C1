#ifndef LVGL_PORT_H
#define LVGL_PORT_H

#include <stdint.h>

#include "esp_err.h"

typedef struct {
    uint16_t hor_res;
    uint16_t ver_res;
    uint8_t rotation;
    uint8_t backlight_percent;
} lvgl_port_cfg_t;

esp_err_t lvgl_port_init(const lvgl_port_cfg_t *cfg);
esp_err_t lvgl_port_lock(uint32_t timeout_ms);
void lvgl_port_unlock(void);
esp_err_t lvgl_port_task_handler(void);

#endif
