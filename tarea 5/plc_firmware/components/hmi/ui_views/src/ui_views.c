#include "ui_views.h"

#include <inttypes.h>
#include <stdio.h>
#include <stdbool.h>

#include "esp_check.h"
#include "lvgl.h"
#include "ui_binding.h"

#define UI_NAV_HEIGHT      36
#define UI_PANEL_RADIUS    6
#define UI_SCREEN_PADDING  10
#define UI_SCREEN_BG       0x111417
#define UI_PANEL_BG        0x1A2127
#define UI_PANEL_ALT_BG    0x222B33
#define UI_ACCENT          0xE6A23C
#define UI_TEXT_PRIMARY    0xF3F4F6
#define UI_TEXT_MUTED      0x8E9AA6
#if LVGL_VERSION_MAJOR >= 9
#define ui_screen_load lv_screen_load
#else
#define ui_screen_load lv_scr_load
#endif

typedef struct {
    lv_obj_t *screen;
    lv_obj_t *title;
    lv_obj_t *subtitle;
    lv_obj_t *value_1;
    lv_obj_t *value_2;
    lv_obj_t *value_3;
    lv_obj_t *value_4;
    lv_obj_t *value_5;
    lv_obj_t *value_6;
} ui_screen_refs_t;

static ui_screen_refs_t s_home;
static ui_screen_refs_t s_states;
static ui_screen_refs_t s_config;
static ui_screen_refs_t s_comm;
static ui_screen_refs_t s_alarms;

static lv_style_t s_style_screen;
static lv_style_t s_style_panel;
static lv_style_t s_style_title;
static lv_style_t s_style_text;
static lv_style_t s_style_muted;
static bool s_created;

static void ui_views_nav_event_cb(lv_event_t *event);

static void ui_views_init_styles(void)
{
    lv_style_init(&s_style_screen);
    lv_style_set_bg_color(&s_style_screen, lv_color_hex(UI_SCREEN_BG));
    lv_style_set_bg_opa(&s_style_screen, LV_OPA_COVER);
    lv_style_set_text_color(&s_style_screen, lv_color_hex(UI_TEXT_PRIMARY));
    lv_style_set_pad_all(&s_style_screen, UI_SCREEN_PADDING);

    lv_style_init(&s_style_panel);
    lv_style_set_bg_color(&s_style_panel, lv_color_hex(UI_PANEL_BG));
    lv_style_set_bg_opa(&s_style_panel, LV_OPA_COVER);
    lv_style_set_border_width(&s_style_panel, 1);
    lv_style_set_border_color(&s_style_panel, lv_color_hex(UI_PANEL_ALT_BG));
    lv_style_set_radius(&s_style_panel, UI_PANEL_RADIUS);
    lv_style_set_pad_all(&s_style_panel, 10);

    lv_style_init(&s_style_title);
    lv_style_set_text_color(&s_style_title, lv_color_hex(UI_ACCENT));
    lv_style_set_text_font(&s_style_title, LV_FONT_DEFAULT);

    lv_style_init(&s_style_text);
    lv_style_set_text_color(&s_style_text, lv_color_hex(UI_TEXT_PRIMARY));

    lv_style_init(&s_style_muted);
    lv_style_set_text_color(&s_style_muted, lv_color_hex(UI_TEXT_MUTED));
}

static lv_obj_t *ui_views_create_nav_button(lv_obj_t *parent, const char *label_text, uint32_t id)
{
    lv_obj_t *button = lv_button_create(parent);
    lv_obj_t *label = lv_label_create(button);

    lv_obj_set_height(button, UI_NAV_HEIGHT - 6);
    lv_obj_set_style_bg_color(button, lv_color_hex(UI_PANEL_ALT_BG), 0);
    lv_obj_set_style_border_width(button, 0, 0);
    lv_obj_set_style_radius(button, 4, 0);
    lv_obj_add_event_cb(button, ui_views_nav_event_cb, LV_EVENT_CLICKED, (void *)(uintptr_t)id);
    lv_label_set_text(label, label_text);
    lv_obj_center(label);
    return button;
}

static lv_obj_t *ui_views_create_panel(lv_obj_t *parent, int32_t height)
{
    lv_obj_t *panel = lv_obj_create(parent);

    lv_obj_remove_style_all(panel);
    lv_obj_add_style(panel, &s_style_panel, 0);
    lv_obj_set_width(panel, LV_PCT(100));
    lv_obj_set_height(panel, height);
    lv_obj_set_scrollbar_mode(panel, LV_SCROLLBAR_MODE_OFF);
    return panel;
}

static void ui_views_fill_nav(lv_obj_t *screen)
{
    lv_obj_t *nav = lv_obj_create(screen);

    lv_obj_remove_style_all(nav);
    lv_obj_set_size(nav, LV_PCT(100), UI_NAV_HEIGHT);
    lv_obj_set_style_bg_opa(nav, LV_OPA_TRANSP, 0);
    lv_obj_set_flex_flow(nav, LV_FLEX_FLOW_ROW);
    lv_obj_set_style_pad_gap(nav, 6, 0);
    lv_obj_set_scrollbar_mode(nav, LV_SCROLLBAR_MODE_OFF);

    (void)ui_views_create_nav_button(nav, "HOME", 0U);
    (void)ui_views_create_nav_button(nav, "STATES", 1U);
    (void)ui_views_create_nav_button(nav, "CONFIG", 2U);
    (void)ui_views_create_nav_button(nav, "COMM", 3U);
    (void)ui_views_create_nav_button(nav, "ALARMS", 4U);
}

static void ui_views_create_screen_shell(ui_screen_refs_t *refs, const char *title, const char *subtitle)
{
    refs->screen = lv_obj_create(NULL);
    lv_obj_remove_style_all(refs->screen);
    lv_obj_add_style(refs->screen, &s_style_screen, 0);
    lv_obj_set_scrollbar_mode(refs->screen, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_style_pad_gap(refs->screen, 8, 0);
    lv_obj_set_flex_flow(refs->screen, LV_FLEX_FLOW_COLUMN);

    ui_views_fill_nav(refs->screen);

    refs->title = lv_label_create(refs->screen);
    lv_obj_add_style(refs->title, &s_style_title, 0);
    lv_label_set_text(refs->title, title);

    refs->subtitle = lv_label_create(refs->screen);
    lv_obj_add_style(refs->subtitle, &s_style_muted, 0);
    lv_label_set_text(refs->subtitle, subtitle);
}

static void ui_views_create_home(void)
{
    lv_obj_t *panel_top;
    lv_obj_t *panel_bottom;

    ui_views_create_screen_shell(&s_home, "PLC HOME", "Industrial HMI");

    panel_top = ui_views_create_panel(s_home.screen, 82);
    panel_bottom = ui_views_create_panel(s_home.screen, 92);

    s_home.value_1 = lv_label_create(panel_top);
    s_home.value_2 = lv_label_create(panel_top);
    s_home.value_3 = lv_label_create(panel_top);
    s_home.value_4 = lv_label_create(panel_bottom);
    s_home.value_5 = lv_label_create(panel_bottom);
    s_home.value_6 = lv_label_create(panel_bottom);

    lv_obj_add_style(s_home.value_1, &s_style_text, 0);
    lv_obj_add_style(s_home.value_2, &s_style_muted, 0);
    lv_obj_add_style(s_home.value_3, &s_style_muted, 0);
    lv_obj_add_style(s_home.value_4, &s_style_text, 0);
    lv_obj_add_style(s_home.value_5, &s_style_text, 0);
    lv_obj_add_style(s_home.value_6, &s_style_muted, 0);

    lv_obj_align(s_home.value_1, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_obj_align(s_home.value_2, LV_ALIGN_TOP_LEFT, 0, 24);
    lv_obj_align(s_home.value_3, LV_ALIGN_TOP_RIGHT, 0, 24);
    lv_obj_align(s_home.value_4, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_obj_align(s_home.value_5, LV_ALIGN_TOP_LEFT, 0, 28);
    lv_obj_align(s_home.value_6, LV_ALIGN_TOP_LEFT, 0, 56);
}

static void ui_views_create_states(void)
{
    lv_obj_t *panel = NULL;

    ui_views_create_screen_shell(&s_states, "I/O STATES", "Realtime snapshot");
    panel = ui_views_create_panel(s_states.screen, 140);

    s_states.value_1 = lv_label_create(panel);
    s_states.value_2 = lv_label_create(panel);
    s_states.value_3 = lv_label_create(panel);
    s_states.value_4 = lv_label_create(panel);
    s_states.value_5 = lv_label_create(panel);
    s_states.value_6 = lv_label_create(panel);

    lv_obj_align(s_states.value_1, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_obj_align(s_states.value_2, LV_ALIGN_TOP_LEFT, 0, 22);
    lv_obj_align(s_states.value_3, LV_ALIGN_TOP_LEFT, 0, 44);
    lv_obj_align(s_states.value_4, LV_ALIGN_TOP_LEFT, 0, 66);
    lv_obj_align(s_states.value_5, LV_ALIGN_TOP_LEFT, 0, 88);
    lv_obj_align(s_states.value_6, LV_ALIGN_TOP_LEFT, 0, 110);
}

static void ui_views_create_config(void)
{
    lv_obj_t *panel = NULL;

    ui_views_create_screen_shell(&s_config, "CONFIG", "Provisioning and runtime");
    panel = ui_views_create_panel(s_config.screen, 140);

    s_config.value_1 = lv_label_create(panel);
    s_config.value_2 = lv_label_create(panel);
    s_config.value_3 = lv_label_create(panel);
    s_config.value_4 = lv_label_create(panel);

    lv_obj_align(s_config.value_1, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_obj_align(s_config.value_2, LV_ALIGN_TOP_LEFT, 0, 26);
    lv_obj_align(s_config.value_3, LV_ALIGN_TOP_LEFT, 0, 52);
    lv_obj_align(s_config.value_4, LV_ALIGN_TOP_LEFT, 0, 78);
}

static void ui_views_create_comm(void)
{
    lv_obj_t *panel = NULL;

    ui_views_create_screen_shell(&s_comm, "COMM", "Connectivity health");
    panel = ui_views_create_panel(s_comm.screen, 140);

    s_comm.value_1 = lv_label_create(panel);
    s_comm.value_2 = lv_label_create(panel);
    s_comm.value_3 = lv_label_create(panel);
    s_comm.value_4 = lv_label_create(panel);
    s_comm.value_5 = lv_label_create(panel);

    lv_obj_align(s_comm.value_1, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_obj_align(s_comm.value_2, LV_ALIGN_TOP_LEFT, 0, 24);
    lv_obj_align(s_comm.value_3, LV_ALIGN_TOP_LEFT, 0, 48);
    lv_obj_align(s_comm.value_4, LV_ALIGN_TOP_LEFT, 0, 72);
    lv_obj_align(s_comm.value_5, LV_ALIGN_TOP_LEFT, 0, 96);
}

static void ui_views_create_alarms(void)
{
    lv_obj_t *panel = NULL;

    ui_views_create_screen_shell(&s_alarms, "ALARMS", "Current active conditions");
    panel = ui_views_create_panel(s_alarms.screen, 140);

    s_alarms.value_1 = lv_label_create(panel);
    s_alarms.value_2 = lv_label_create(panel);
    s_alarms.value_3 = lv_label_create(panel);

    lv_obj_align(s_alarms.value_1, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_obj_align(s_alarms.value_2, LV_ALIGN_TOP_LEFT, 0, 26);
    lv_obj_align(s_alarms.value_3, LV_ALIGN_TOP_LEFT, 0, 52);
}

static const char *ui_views_status_text(bool ok)
{
    return ok ? "OK" : "WAIT";
}

static void ui_views_refresh_home(const ui_model_t *model)
{
    lv_label_set_text_fmt(s_home.value_1, "%s", model->project_name);
    lv_label_set_text_fmt(s_home.value_2, "IP %s", model->ip_addr);
    lv_label_set_text_fmt(s_home.value_3, "TIME %s", model->time_str);
    lv_label_set_text_fmt(s_home.value_4, "AI0 %.2f  AI1 %.2f", model->io.ai[0], model->io.ai[1]);
    lv_label_set_text_fmt(s_home.value_5, "P %.1fW  V %.1fV  I %.2fA", model->io.energy_power, model->io.energy_voltage, model->io.energy_current);
    lv_label_set_text_fmt(s_home.value_6, "ALARMS %u  WIFI %s", model->alarm_count, ui_views_status_text(model->wifi_ok));
}

static void ui_views_refresh_states(const ui_model_t *model)
{
    lv_label_set_text_fmt(s_states.value_1, "DI  0x%08" PRIX32, model->io.di_bitmap);
    lv_label_set_text_fmt(s_states.value_2, "DO  0x%08" PRIX32, model->io.do_bitmap);
    lv_label_set_text_fmt(s_states.value_3, "AI0 %.2f", model->io.ai[0]);
    lv_label_set_text_fmt(s_states.value_4, "AI1 %.2f", model->io.ai[1]);
    lv_label_set_text_fmt(s_states.value_5, "AO0 %.2f", model->io.ao[0]);
    lv_label_set_text_fmt(s_states.value_6, "AO1 %.2f", model->io.ao[1]);
}

static void ui_views_refresh_config(const ui_model_t *model)
{
    lv_label_set_text_fmt(s_config.value_1, "DISPLAY 320x240 DARK");
    lv_label_set_text_fmt(s_config.value_2, "PROJECT %s", model->project_name);
    lv_label_set_text_fmt(s_config.value_3, "WEB CONFIG %s", ui_views_status_text(model->web_ok));
    lv_label_set_text_fmt(s_config.value_4, "BLE CONFIG READY");
}

static void ui_views_refresh_comm(const ui_model_t *model)
{
    lv_label_set_text_fmt(s_comm.value_1, "WIFI   %s", ui_views_status_text(model->wifi_ok));
    lv_label_set_text_fmt(s_comm.value_2, "WEB    %s", ui_views_status_text(model->web_ok));
    lv_label_set_text_fmt(s_comm.value_3, "MODBUS %s", ui_views_status_text(model->modbus_ok));
    lv_label_set_text_fmt(s_comm.value_4, "MQTT   %s", ui_views_status_text(model->mqtt_ok));
    lv_label_set_text_fmt(s_comm.value_5, "IP %s", model->ip_addr);
}

static void ui_views_refresh_alarms(const ui_model_t *model)
{
    lv_label_set_text_fmt(s_alarms.value_1, "ACTIVE %u", model->alarm_count);
    lv_label_set_text_fmt(s_alarms.value_2, "POWER %.1fW  FREQ %.1fHz", model->io.energy_power, model->io.energy_frequency);
    lv_label_set_text_fmt(s_alarms.value_3, "%s", model->alarm_count > 0U ? "CHECK FIELD INPUTS" : "SYSTEM STABLE");
}

static void ui_views_nav_event_cb(lv_event_t *event)
{
    uintptr_t screen_id = (uintptr_t)lv_event_get_user_data(event);

    switch (screen_id) {
    case 0U:
        (void)ui_views_show_home();
        break;
    case 1U:
        (void)ui_views_show_states();
        break;
    case 2U:
        (void)ui_views_show_config();
        break;
    case 3U:
        (void)ui_views_show_comm();
        break;
    case 4U:
        (void)ui_views_show_alarms();
        break;
    default:
        break;
    }
}

esp_err_t ui_views_create_all(void)
{
    if (s_created) {
        return ESP_OK;
    }

    ui_views_init_styles();
    ui_views_create_home();
    ui_views_create_states();
    ui_views_create_config();
    ui_views_create_comm();
    ui_views_create_alarms();
    s_created = true;
    return ui_views_refresh();
}

esp_err_t ui_views_refresh(void)
{
    const ui_model_t *model = ui_binding_get_model();

    ESP_RETURN_ON_FALSE(s_created, ESP_ERR_INVALID_STATE, "ui_views", "not created");
    ESP_RETURN_ON_FALSE(model != NULL, ESP_ERR_INVALID_STATE, "ui_views", "model null");

    ui_views_refresh_home(model);
    ui_views_refresh_states(model);
    ui_views_refresh_config(model);
    ui_views_refresh_comm(model);
    ui_views_refresh_alarms(model);
    return ESP_OK;
}

esp_err_t ui_views_show_home(void)
{
    ESP_RETURN_ON_FALSE(s_created, ESP_ERR_INVALID_STATE, "ui_views", "not created");
    ui_screen_load(s_home.screen);
    return ESP_OK;
}

esp_err_t ui_views_show_states(void)
{
    ESP_RETURN_ON_FALSE(s_created, ESP_ERR_INVALID_STATE, "ui_views", "not created");
    ui_screen_load(s_states.screen);
    return ESP_OK;
}

esp_err_t ui_views_show_config(void)
{
    ESP_RETURN_ON_FALSE(s_created, ESP_ERR_INVALID_STATE, "ui_views", "not created");
    ui_screen_load(s_config.screen);
    return ESP_OK;
}

esp_err_t ui_views_show_comm(void)
{
    ESP_RETURN_ON_FALSE(s_created, ESP_ERR_INVALID_STATE, "ui_views", "not created");
    ui_screen_load(s_comm.screen);
    return ESP_OK;
}

esp_err_t ui_views_show_alarms(void)
{
    ESP_RETURN_ON_FALSE(s_created, ESP_ERR_INVALID_STATE, "ui_views", "not created");
    ui_screen_load(s_alarms.screen);
    return ESP_OK;
}
