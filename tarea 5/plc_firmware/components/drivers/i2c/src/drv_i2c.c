#include "drv_i2c.h"

#include <string.h>

#include "driver/i2c_master.h"
#include "esp_check.h"
#include "esp_log.h"

static const char *TAG = "drv_i2c";

static driver_health_t s_health = {
    .initialized = false,
    .status = DRIVER_STATUS_UNINIT,
    .last_error = ESP_OK,
};

static i2c_master_bus_handle_t s_bus = NULL;
static drv_i2c_cfg_t s_cfg = {0};

static esp_err_t drv_i2c_create_dev(uint8_t dev_addr, i2c_master_dev_handle_t *out_dev)
{
    ESP_RETURN_ON_FALSE(s_health.initialized, ESP_ERR_INVALID_STATE, TAG, "I2C no inicializado");
    ESP_RETURN_ON_FALSE(out_dev != NULL, ESP_ERR_INVALID_ARG, TAG, "out_dev es NULL");

    i2c_device_config_t dev_cfg = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = dev_addr,
        .scl_speed_hz = s_cfg.freq_hz,
    };

    return i2c_master_bus_add_device(s_bus, &dev_cfg, out_dev);
}

esp_err_t drv_i2c_init(const drv_i2c_cfg_t *cfg)
{
    ESP_RETURN_ON_FALSE(cfg != NULL, ESP_ERR_INVALID_ARG, TAG, "cfg es NULL");

    if (s_health.initialized) {
        ESP_LOGW(TAG, "I2C ya estaba inicializado");
        return ESP_OK;
    }

    memset(&s_cfg, 0, sizeof(s_cfg));
    memcpy(&s_cfg, cfg, sizeof(s_cfg));

    i2c_master_bus_config_t bus_cfg = {
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .i2c_port = s_cfg.port,
        .scl_io_num = s_cfg.gpio_scl,
        .sda_io_num = s_cfg.gpio_sda,
        .glitch_ignore_cnt = 7,
        .flags.enable_internal_pullup = s_cfg.pullup_en,
    };

    esp_err_t err = i2c_new_master_bus(&bus_cfg, &s_bus);
    if (err != ESP_OK) {
        s_health.initialized = false;
        s_health.status = DRIVER_STATUS_ERROR;
        s_health.last_error = err;
        ESP_LOGE(TAG, "Error creando bus I2C: %s", esp_err_to_name(err));
        return err;
    }

    s_health.initialized = true;
    s_health.status = DRIVER_STATUS_READY;
    s_health.last_error = ESP_OK;

    ESP_LOGI(TAG, "I2C listo: port=%d SDA=%d SCL=%d freq=%lu",
             s_cfg.port, s_cfg.gpio_sda, s_cfg.gpio_scl, (unsigned long)s_cfg.freq_hz);

    return ESP_OK;
}

esp_err_t drv_i2c_deinit(void)
{
    if (!s_health.initialized) {
        return ESP_OK;
    }

    esp_err_t err = i2c_del_master_bus(s_bus);
    if (err != ESP_OK) {
        s_health.status = DRIVER_STATUS_ERROR;
        s_health.last_error = err;
        ESP_LOGE(TAG, "Error cerrando bus I2C: %s", esp_err_to_name(err));
        return err;
    }

    s_bus = NULL;
    memset(&s_cfg, 0, sizeof(s_cfg));

    s_health.initialized = false;
    s_health.status = DRIVER_STATUS_UNINIT;
    s_health.last_error = ESP_OK;

    return ESP_OK;
}

esp_err_t drv_i2c_write(uint8_t dev_addr,
                        const uint8_t *data,
                        size_t len,
                        uint32_t timeout_ms)
{
    ESP_RETURN_ON_FALSE(data != NULL, ESP_ERR_INVALID_ARG, TAG, "data es NULL");
    ESP_RETURN_ON_FALSE(len > 0U, ESP_ERR_INVALID_ARG, TAG, "len invalido");

    i2c_master_dev_handle_t dev = NULL;
    esp_err_t err = drv_i2c_create_dev(dev_addr, &dev);
    ESP_RETURN_ON_ERROR(err, TAG, "No se pudo crear dispositivo I2C");

    err = i2c_master_transmit(dev, data, len, timeout_ms);
    i2c_master_bus_rm_device(dev);

    s_health.status = (err == ESP_OK) ? DRIVER_STATUS_READY : DRIVER_STATUS_ERROR;
    s_health.last_error = err;

    return err;
}

esp_err_t drv_i2c_read(uint8_t dev_addr,
                       uint8_t *data,
                       size_t len,
                       uint32_t timeout_ms)
{
    ESP_RETURN_ON_FALSE(data != NULL, ESP_ERR_INVALID_ARG, TAG, "data es NULL");
    ESP_RETURN_ON_FALSE(len > 0U, ESP_ERR_INVALID_ARG, TAG, "len invalido");

    i2c_master_dev_handle_t dev = NULL;
    esp_err_t err = drv_i2c_create_dev(dev_addr, &dev);
    ESP_RETURN_ON_ERROR(err, TAG, "No se pudo crear dispositivo I2C");

    err = i2c_master_receive(dev, data, len, timeout_ms);
    i2c_master_bus_rm_device(dev);

    s_health.status = (err == ESP_OK) ? DRIVER_STATUS_READY : DRIVER_STATUS_ERROR;
    s_health.last_error = err;

    return err;
}

esp_err_t drv_i2c_write_read(uint8_t dev_addr,
                             const uint8_t *tx_data,
                             size_t tx_len,
                             uint8_t *rx_data,
                             size_t rx_len,
                             uint32_t timeout_ms)
{
    ESP_RETURN_ON_FALSE(tx_data != NULL, ESP_ERR_INVALID_ARG, TAG, "tx_data es NULL");
    ESP_RETURN_ON_FALSE(rx_data != NULL, ESP_ERR_INVALID_ARG, TAG, "rx_data es NULL");
    ESP_RETURN_ON_FALSE(tx_len > 0U, ESP_ERR_INVALID_ARG, TAG, "tx_len invalido");
    ESP_RETURN_ON_FALSE(rx_len > 0U, ESP_ERR_INVALID_ARG, TAG, "rx_len invalido");

    i2c_master_dev_handle_t dev = NULL;
    esp_err_t err = drv_i2c_create_dev(dev_addr, &dev);
    ESP_RETURN_ON_ERROR(err, TAG, "No se pudo crear dispositivo I2C");

    err = i2c_master_transmit_receive(dev, tx_data, tx_len, rx_data, rx_len, timeout_ms);
    i2c_master_bus_rm_device(dev);

    s_health.status = (err == ESP_OK) ? DRIVER_STATUS_READY : DRIVER_STATUS_ERROR;
    s_health.last_error = err;

    return err;
}

esp_err_t drv_i2c_probe(uint8_t dev_addr, uint32_t timeout_ms)
{
    ESP_RETURN_ON_FALSE(s_health.initialized, ESP_ERR_INVALID_STATE, TAG, "I2C no inicializado");

    esp_err_t err = i2c_master_probe(s_bus, dev_addr, timeout_ms);
    s_health.status = (err == ESP_OK) ? DRIVER_STATUS_READY : DRIVER_STATUS_WARN;
    s_health.last_error = err;

    return err;
}

driver_health_t drv_i2c_get_health(void)
{
    return s_health;
}
