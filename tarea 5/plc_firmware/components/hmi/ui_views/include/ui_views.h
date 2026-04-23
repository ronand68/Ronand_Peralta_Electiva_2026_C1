#ifndef UI_VIEWS_H
#define UI_VIEWS_H

#include "esp_err.h"

esp_err_t ui_views_create_all(void);
esp_err_t ui_views_refresh(void);
esp_err_t ui_views_show_home(void);
esp_err_t ui_views_show_states(void);
esp_err_t ui_views_show_config(void);
esp_err_t ui_views_show_comm(void);
esp_err_t ui_views_show_alarms(void);

#endif
