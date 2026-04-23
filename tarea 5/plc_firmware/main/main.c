#include <inttypes.h>
#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_log.h"

#include "board_io_map.h"
#include "drv_adc_ext.h"
#include "drv_ao_0_10v.h"
#include "drv_ao_4_20ma.h"
#include "drv_din.h"
#include "drv_dout.h"
#include "drv_energy.h"
#include "drv_i2c.h"
#include "drv_lcd.h"
#include "drv_sdcard.h"
#include "drv_touch.h"
#include "drv_usb_device.h"
#include "ble_config_service.h"
#include "lvgl_port.h"
#include "plc_core.h"
#include "ui_binding.h"
#include "ui_views.h"
#include "web_config_service.h"
#include "wifi_manager.h"

static const char *TAG = "PLC_MAIN";

#define UI_WIDTH             320U
#define UI_HEIGHT            240U
#define UI_HANDLER_PERIOD_MS 20U

static void ui_task(void *arg)
{
    (void)arg;

    while (1) {
        (void)lvgl_port_task_handler();
        vTaskDelay(pdMS_TO_TICKS(UI_HANDLER_PERIOD_MS));
    }
}

void app_main(void)
{
    drv_i2c_cfg_t i2c_cfg = {
        .port = BOARD_I2C_PORT,
        .gpio_sda = BOARD_I2C_SDA_GPIO,
        .gpio_scl = BOARD_I2C_SCL_GPIO,
        .freq_hz = BOARD_I2C_FREQ_HZ,
        .pullup_en = true
    };

    drv_din_cfg_t din_cfg = {
        .channel_count = 6,
        .gpio_list = {
            BOARD_DI_NPN1_GPIO,
            BOARD_DI_NPN2_GPIO,
            BOARD_DI_NPN3_GPIO,
            BOARD_DI_NPN4_GPIO,
            BOARD_DI_PNP1_GPIO,
            BOARD_DI_PNP2_GPIO
        },
        .invert = { false, false, false, false, false, false }
    };

    drv_dout_cfg_t dout_cfg = {
        .channel_count = 8,
        .gpio_list = {
            BOARD_DO_NPN1_GPIO,
            BOARD_DO_NPN2_GPIO,
            BOARD_DO_NPN3_GPIO,
            BOARD_DO_NPN4_GPIO,
            BOARD_DO_RLY1_GPIO,
            BOARD_DO_RLY2_GPIO,
            BOARD_DO_PNP1_GPIO,
            BOARD_DO_PNP2_GPIO
        },
        .invert = { false, false, false, false, false, false, false, false },
        .default_state = { false, false, false, false, false, false, false, false }
    };

    drv_adc_ext_cfg_t adc_cfg = {
        .channel_count = 2,
        .gain = { 1.0f, 1.0f },
        .offset = { 0.0f, 0.0f }
    };

    drv_energy_cfg_t energy_cfg = {
        .refresh_ms = 500
    };

    drv_ao_4_20ma_cfg_t ao_420_cfg = {
        .channel_count = 2,
        .min_engineering = { 0.0f, 0.0f },
        .max_engineering = { 100.0f, 100.0f }
    };

    drv_ao_0_10v_cfg_t ao_010_cfg = {
        .channel_count = 2,
        .min_engineering = { 0.0f, 0.0f },
        .max_engineering = { 100.0f, 100.0f }
    };

    drv_lcd_cfg_t lcd_cfg = {
        .width = UI_WIDTH,
        .height = UI_HEIGHT,
        .rotation = 0,
        .double_buffer = true
    };

    drv_touch_cfg_t touch_cfg = {
        .width = UI_WIDTH,
        .height = UI_HEIGHT,
        .swap_xy = false,
        .mirror_x = false,
        .mirror_y = false
    };

    lvgl_port_cfg_t lvgl_cfg = {
        .hor_res = UI_WIDTH,
        .ver_res = UI_HEIGHT,
        .rotation = 0,
        .backlight_percent = 70
    };

    drv_usb_device_cfg_t usb_cfg = {
        .device_class = USB_DEVICE_CLASS_COMPOSITE,
        .enable_cdc = true,
        .enable_msc = true,
        .manufacturer = "ITLA",
        .product = "PLC Skeleton",
        .serial = "PLC-ESP32S3-001"
    };

    drv_sdcard_cfg_t sdcard_cfg = {
        .mode = DRV_SDCARD_MODE_SDSPI,
        .mount_point = "/sdcard",
        .format_if_mount_failed = false
    };

    wifi_manager_cfg_t wifi_cfg = {
        .mode = WIFI_MANAGER_MODE_APSTA,
        .sta_ssid = "TODO_VERIFY_STA",
        .sta_pass = "TODO_VERIFY_STA_PASS",
        .ap_ssid = "PLC-CONFIG",
        .ap_pass = "plc12345",
        .ap_channel = 6
    };

    web_config_service_cfg_t web_cfg = {
        .port = 80,
        .enable_auth = false
    };

    ble_config_service_cfg_t ble_cfg = {
        .device_name = "PLC-CONFIG",
        .enable_provisioning = true
    };

    plc_io_snapshot_t snap = {0};
    drv_usb_device_status_t usb_status = {0};
    drv_sdcard_status_t sd_status = {0};
    wifi_manager_status_t wifi_status = {0};
    web_config_service_status_t web_status = {0};
    ble_config_service_status_t ble_status = {0};
    char uptime_str[16] = {0};
    const char *ui_ip_text = "TODO_VERIFY";

    ESP_LOGI(TAG, "==== PLC start ====");

    ESP_ERROR_CHECK(drv_i2c_init(&i2c_cfg));
    ESP_ERROR_CHECK(drv_din_init(&din_cfg));
    ESP_ERROR_CHECK(drv_dout_init(&dout_cfg));
    ESP_ERROR_CHECK(drv_adc_ext_init(&adc_cfg));
    ESP_ERROR_CHECK(drv_energy_init(&energy_cfg));
    ESP_ERROR_CHECK(drv_ao_4_20ma_init(&ao_420_cfg));
    ESP_ERROR_CHECK(drv_ao_0_10v_init(&ao_010_cfg));
    ESP_ERROR_CHECK(drv_lcd_init(&lcd_cfg));
    ESP_ERROR_CHECK(drv_touch_init(&touch_cfg));
    ESP_ERROR_CHECK(drv_usb_device_init(&usb_cfg));
    ESP_ERROR_CHECK(drv_sdcard_init(&sdcard_cfg));
    ESP_ERROR_CHECK(wifi_manager_init(&wifi_cfg));
    ESP_ERROR_CHECK(web_config_service_init(&web_cfg));
    ESP_ERROR_CHECK(ble_config_service_init(&ble_cfg));

    ESP_ERROR_CHECK(drv_usb_device_start());
    ESP_ERROR_CHECK(drv_sdcard_mount());
    ESP_ERROR_CHECK(wifi_manager_start());
    ESP_ERROR_CHECK(web_config_service_start());
    ESP_ERROR_CHECK(ble_config_service_start());

    ESP_ERROR_CHECK(plc_core_init());
    ESP_ERROR_CHECK(ui_binding_init());
    ESP_ERROR_CHECK(lvgl_port_init(&lvgl_cfg));
    ESP_ERROR_CHECK(lvgl_port_lock(100U));
    ESP_ERROR_CHECK(ui_views_create_all());
    ESP_ERROR_CHECK(ui_views_show_home());
    lvgl_port_unlock();
    xTaskCreate(ui_task, "ui_task", 4096, NULL, 4, NULL);

    while (1) {
        uint32_t uptime_seconds = (uint32_t)(xTaskGetTickCount() / configTICK_RATE_HZ);

        ESP_ERROR_CHECK(plc_core_update());
        ESP_ERROR_CHECK(plc_core_get_snapshot(&snap));
        ESP_ERROR_CHECK(drv_usb_device_get_status(&usb_status));
        ESP_ERROR_CHECK(drv_sdcard_get_status(&sd_status));
        ESP_ERROR_CHECK(wifi_manager_get_status(&wifi_status));
        ESP_ERROR_CHECK(web_config_service_get_status(&web_status));
        ESP_ERROR_CHECK(ble_config_service_get_status(&ble_status));

        snprintf(uptime_str, sizeof(uptime_str), "%02" PRIu32 ":%02" PRIu32 ":%02" PRIu32,
                 uptime_seconds / 3600U,
                 (uptime_seconds / 60U) % 60U,
                 uptime_seconds % 60U);
        if (wifi_status.ap_active) {
            ui_ip_text = "TODO_VERIFY_AP";
        } else if (wifi_status.sta_connected) {
            ui_ip_text = "TODO_VERIFY_STA";
        } else {
            ui_ip_text = "NO LINK";
        }

        ESP_ERROR_CHECK(ui_binding_update_snapshot(&snap));
        ESP_ERROR_CHECK(ui_binding_set_runtime_text("PLC ESP32-S3", ui_ip_text, uptime_str));
        ESP_ERROR_CHECK(ui_binding_set_comm_status(wifi_status.sta_connected || wifi_status.ap_active,
                                                  web_status.running,
                                                  false,
                                                  false));
        ESP_ERROR_CHECK(ui_binding_set_alarm_count(0U));
        ESP_ERROR_CHECK(lvgl_port_lock(50U));
        ESP_ERROR_CHECK(ui_views_refresh());
        lvgl_port_unlock();

        ESP_LOGI(TAG,
                 "DI=0x%08" PRIX32
                 " DO=0x%08" PRIX32
                 " AI0=%.2f AI1=%.2f"
                 " AO0=%.2f AO1=%.2f"
                 " V=%.2f I=%.2f P=%.2f F=%.2f"
                 " USB(start=%d msc=%d tx=%" PRIu32 ")"
                 " SD(mounted=%d free=%" PRIu64 ")"
                 " WIFI(sta=%d ap=%d rssi=%d)"
                 " WEB(run=%d req=%" PRIu32 ")"
                 " BLE(run=%d conn=%d)",
                 snap.di_bitmap,
                 snap.do_bitmap,
                 snap.ai[0], snap.ai[1],
                 snap.ao[0], snap.ao[1],
                 snap.energy_voltage,
                 snap.energy_current,
                 snap.energy_power,
                 snap.energy_frequency,
                 usb_status.started,
                 usb_status.mounted,
                 usb_status.tx_bytes,
                 sd_status.mounted,
                 sd_status.free_bytes,
                 wifi_status.sta_connected,
                 wifi_status.ap_active,
                 wifi_status.rssi,
                 web_status.running,
                 web_status.request_count,
                 ble_status.running,
                 ble_status.connected);

        vTaskDelay(pdMS_TO_TICKS(500));
    }
}
