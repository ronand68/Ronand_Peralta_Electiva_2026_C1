#ifndef DRV_USB_DEVICE_H
#define DRV_USB_DEVICE_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "esp_err.h"
#include "plc_types.h"

typedef enum {
    USB_DEVICE_CLASS_NONE = 0,
    USB_DEVICE_CLASS_CDC,
    USB_DEVICE_CLASS_MSC,
    USB_DEVICE_CLASS_COMPOSITE
} drv_usb_device_class_t;

typedef struct {
    drv_usb_device_class_t device_class;
    bool enable_cdc;
    bool enable_msc;
    char manufacturer[32];
    char product[32];
    char serial[32];
} drv_usb_device_cfg_t;

typedef struct {
    bool started;
    bool mounted;
    uint32_t tx_bytes;
    uint32_t rx_bytes;
    esp_err_t last_error;
} drv_usb_device_status_t;

esp_err_t drv_usb_device_init(const drv_usb_device_cfg_t *cfg);
esp_err_t drv_usb_device_start(void);
esp_err_t drv_usb_device_stop(void);
esp_err_t drv_usb_device_cdc_write(const uint8_t *data, size_t len);
esp_err_t drv_usb_device_cdc_read(uint8_t *data, size_t max_len, size_t *out_len);
esp_err_t drv_usb_device_get_status(drv_usb_device_status_t *status);
driver_health_t drv_usb_device_get_health(void);

#endif
