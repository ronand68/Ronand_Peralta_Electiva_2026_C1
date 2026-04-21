#include "plc_core.h"

#include <string.h>

#include "diac_bridge.h"
#include "drv_adc_ext.h"
#include "drv_ao_0_10v.h"
#include "drv_ao_4_20ma.h"
#include "drv_din.h"
#include "drv_dout.h"
#include "drv_energy.h"
#include "esp_check.h"
#include "esp_log.h"
#include "openplc_bridge.h"

static const char *TAG = "plc_core";

static plc_io_snapshot_t s_snapshot;

esp_err_t plc_core_init(void)
{
    memset(&s_snapshot, 0, sizeof(s_snapshot));

    ESP_RETURN_ON_ERROR(openplc_bridge_init(), TAG, "openplc_bridge_init fallo");
    ESP_RETURN_ON_ERROR(diac_bridge_init(), TAG, "diac_bridge_init fallo");

    ESP_RETURN_ON_ERROR(openplc_bridge_start(), TAG, "openplc_bridge_start fallo");
    ESP_RETURN_ON_ERROR(diac_bridge_start(), TAG, "diac_bridge_start fallo");

    ESP_LOGI(TAG, "PLC core inicializado");
    return ESP_OK;
}

esp_err_t plc_core_update(void)
{
    drv_energy_data_t energy = {0};

    ESP_RETURN_ON_ERROR(drv_din_update(), TAG, "drv_din_update fallo");

    s_snapshot.di_bitmap = drv_din_get_bitmap();
    s_snapshot.do_bitmap = drv_dout_get_bitmap();

    ESP_RETURN_ON_ERROR(drv_adc_ext_update(), TAG, "drv_adc_ext_update fallo");

    for (int i = 0; i < PLC_MAX_AI_CHANNELS; i++) {
        s_snapshot.ai[i] = drv_adc_ext_get_scaled((uint8_t)i);
    }

    for (int i = 0; i < PLC_MAX_AO_CHANNELS; i++) {
        s_snapshot.ao[i] = drv_ao_0_10v_get_voltage((uint8_t)i);
    }

    ESP_RETURN_ON_ERROR(drv_energy_update(), TAG, "drv_energy_update fallo");
    ESP_RETURN_ON_ERROR(drv_energy_get_data(&energy), TAG, "drv_energy_get_data fallo");

    s_snapshot.energy_voltage = energy.voltage;
    s_snapshot.energy_current = energy.current;
    s_snapshot.energy_power = energy.power;
    s_snapshot.energy_frequency = energy.frequency;

    ESP_RETURN_ON_ERROR(openplc_bridge_push_io(&s_snapshot), TAG, "openplc_bridge_push_io fallo");
    ESP_RETURN_ON_ERROR(diac_bridge_push_io(&s_snapshot), TAG, "diac_bridge_push_io fallo");

    return ESP_OK;
}

esp_err_t plc_core_get_snapshot(plc_io_snapshot_t *snapshot)
{
    ESP_RETURN_ON_FALSE(snapshot != NULL, ESP_ERR_INVALID_ARG, TAG, "snapshot null");
    *snapshot = s_snapshot;
    return ESP_OK;
}
