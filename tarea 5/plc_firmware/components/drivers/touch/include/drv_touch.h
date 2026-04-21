#ifndef DRV_TOUCH_H
#define DRV_TOUCH_H

#include <stdbool.h>
#include <stdint.h>

#include "esp_err.h"
#include "plc_types.h"

typedef struct {
    uint16_t width;
    uint16_t height;
    bool swap_xy;
    bool mirror_x;
    bool mirror_y;
} drv_touch_cfg_t;

typedef struct {
    bool pressed;
    uint16_t x;
    uint16_t y;
} drv_touch_point_t;

esp_err_t drv_touch_init(const drv_touch_cfg_t *cfg);
esp_err_t drv_touch_update(void);
esp_err_t drv_touch_get_point(drv_touch_point_t *point);
esp_err_t drv_touch_mock_set(bool pressed, uint16_t x, uint16_t y);
driver_health_t drv_touch_get_health(void);

#endif
