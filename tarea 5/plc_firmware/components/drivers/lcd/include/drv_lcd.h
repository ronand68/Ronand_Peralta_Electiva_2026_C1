#ifndef DRV_LCD_H
#define DRV_LCD_H

#include <stdbool.h>
#include <stdint.h>

#include "esp_err.h"
#include "plc_types.h"

typedef struct {
    uint16_t width;
    uint16_t height;
    uint8_t rotation;
    bool double_buffer;
} drv_lcd_cfg_t;

esp_err_t drv_lcd_init(const drv_lcd_cfg_t *cfg);
esp_err_t drv_lcd_clear(uint16_t color);
esp_err_t drv_lcd_set_backlight(uint8_t percent);
esp_err_t drv_lcd_draw_pixel(uint16_t x, uint16_t y, uint16_t color);
esp_err_t drv_lcd_present(void);
driver_health_t drv_lcd_get_health(void);

#endif
