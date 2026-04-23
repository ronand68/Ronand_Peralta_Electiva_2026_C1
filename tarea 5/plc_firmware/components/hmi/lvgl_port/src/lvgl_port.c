#include "lvgl_port.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "esp_check.h"
#include "esp_heap_caps.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "drv_lcd.h"
#include "drv_touch.h"
#include "lvgl.h"

#define LVGL_PORT_TICK_MS        5U
#define LVGL_PORT_DRAW_BUF_LINES 20U

static const char *TAG = "lvgl_port";

static SemaphoreHandle_t s_lvgl_mutex;
static esp_timer_handle_t s_tick_timer;
static lv_display_t *s_display;
static lv_indev_t *s_touch_indev;
static lvgl_port_cfg_t s_cfg;
static void *s_draw_buf_1;
static void *s_draw_buf_2;
static bool s_initialized;

static void lvgl_port_tick_cb(void *arg)
{
    (void)arg;
    lv_tick_inc(LVGL_PORT_TICK_MS);
}

static uint32_t lvgl_port_timeout_to_ticks(uint32_t timeout_ms)
{
    if (timeout_ms == UINT32_MAX) {
        return portMAX_DELAY;
    }
    return pdMS_TO_TICKS(timeout_ms);
}

static void lvgl_port_flush_cb(lv_display_t *display, const lv_area_t *area, uint8_t *px_map)
{
    uint32_t color_index = 0;

    for (int32_t y = area->y1; y <= area->y2; y++) {
        for (int32_t x = area->x1; x <= area->x2; x++) {
            uint16_t color = 0U;

            memcpy(&color, px_map + (color_index * sizeof(uint16_t)), sizeof(color));
            (void)drv_lcd_draw_pixel((uint16_t)x, (uint16_t)y, color);
            color_index++;
        }
    }

    (void)drv_lcd_present();
    lv_display_flush_ready(display);
}

static void lvgl_port_touch_read_cb(lv_indev_t *indev, lv_indev_data_t *data)
{
    drv_touch_point_t point = {0};

    (void)indev;

    if (drv_touch_update() != ESP_OK || drv_touch_get_point(&point) != ESP_OK) {
        data->state = LV_INDEV_STATE_RELEASED;
        return;
    }

    if (point.pressed) {
        if (point.x >= s_cfg.hor_res) {
            point.x = s_cfg.hor_res - 1U;
        }
        if (point.y >= s_cfg.ver_res) {
            point.y = s_cfg.ver_res - 1U;
        }
        data->state = LV_INDEV_STATE_PRESSED;
        data->point.x = point.x;
        data->point.y = point.y;
    } else {
        data->state = LV_INDEV_STATE_RELEASED;
    }
}

esp_err_t lvgl_port_init(const lvgl_port_cfg_t *cfg)
{
    const esp_timer_create_args_t tick_timer_args = {
        .callback = lvgl_port_tick_cb,
        .name = "lvgl_tick"
    };
    size_t draw_buffer_sz = 0U;

    ESP_RETURN_ON_FALSE(cfg != NULL, ESP_ERR_INVALID_ARG, TAG, "cfg null");
    ESP_RETURN_ON_FALSE(cfg->hor_res > 0U && cfg->ver_res > 0U, ESP_ERR_INVALID_ARG, TAG, "invalid resolution");

    if (s_initialized) {
        return ESP_OK;
    }

    s_cfg = *cfg;
    s_lvgl_mutex = xSemaphoreCreateMutex();
    ESP_RETURN_ON_FALSE(s_lvgl_mutex != NULL, ESP_ERR_NO_MEM, TAG, "mutex alloc failed");

    lv_init();

    draw_buffer_sz = cfg->hor_res * LVGL_PORT_DRAW_BUF_LINES * sizeof(uint16_t);
    s_draw_buf_1 = heap_caps_malloc(draw_buffer_sz, MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);
    s_draw_buf_2 = heap_caps_malloc(draw_buffer_sz, MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);
    ESP_RETURN_ON_FALSE(s_draw_buf_1 != NULL && s_draw_buf_2 != NULL, ESP_ERR_NO_MEM, TAG, "draw buffer alloc failed");

    s_display = lv_display_create(cfg->hor_res, cfg->ver_res);
    ESP_RETURN_ON_FALSE(s_display != NULL, ESP_FAIL, TAG, "display create failed");

    lv_display_set_buffers(s_display, s_draw_buf_1, s_draw_buf_2, draw_buffer_sz, LV_DISPLAY_RENDER_MODE_PARTIAL);
    lv_display_set_color_format(s_display, LV_COLOR_FORMAT_RGB565);
    lv_display_set_flush_cb(s_display, lvgl_port_flush_cb);
    lv_display_set_rotation(s_display, cfg->rotation);

    s_touch_indev = lv_indev_create();
    ESP_RETURN_ON_FALSE(s_touch_indev != NULL, ESP_FAIL, TAG, "touch indev create failed");
    lv_indev_set_type(s_touch_indev, LV_INDEV_TYPE_POINTER);
    lv_indev_set_display(s_touch_indev, s_display);
    lv_indev_set_read_cb(s_touch_indev, lvgl_port_touch_read_cb);

    ESP_RETURN_ON_ERROR(drv_lcd_clear(0x0000), TAG, "lcd clear failed");
    ESP_RETURN_ON_ERROR(drv_lcd_set_backlight(cfg->backlight_percent), TAG, "backlight failed");
    ESP_RETURN_ON_ERROR(esp_timer_create(&tick_timer_args, &s_tick_timer), TAG, "tick timer create failed");
    ESP_RETURN_ON_ERROR(esp_timer_start_periodic(s_tick_timer, LVGL_PORT_TICK_MS * 1000U), TAG, "tick timer start failed");

    s_initialized = true;
    ESP_LOGI(TAG, "LVGL port ready %ux%u", cfg->hor_res, cfg->ver_res);
    return ESP_OK;
}

esp_err_t lvgl_port_lock(uint32_t timeout_ms)
{
    ESP_RETURN_ON_FALSE(s_initialized, ESP_ERR_INVALID_STATE, TAG, "not init");
    return xSemaphoreTake(s_lvgl_mutex, lvgl_port_timeout_to_ticks(timeout_ms)) == pdTRUE ? ESP_OK : ESP_ERR_TIMEOUT;
}

void lvgl_port_unlock(void)
{
    if (s_initialized && s_lvgl_mutex != NULL) {
        (void)xSemaphoreGive(s_lvgl_mutex);
    }
}

esp_err_t lvgl_port_task_handler(void)
{
    esp_err_t err = lvgl_port_lock(20U);
    ESP_RETURN_ON_ERROR(err, TAG, "lock failed");
    (void)lv_timer_handler();
    lvgl_port_unlock();
    return ESP_OK;
}
