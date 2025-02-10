#include <stdio.h>
#include <string.h>

#include "control_light/control_light.h"
#include "esp_log.h"
#include "filesystem/filesystem.h"
#include "primary_screen.h"
#include "utils/utils.h"

static const char *PRIMARY_TAG = "primary_screen";

widget_array *curr_widgets = NULL;

typedef struct {
  char *name;
  lv_obj_t *screen;
} screen_data;

void show_control(lv_event_t *event) {
  screen_data *data = (screen_data *)lv_event_get_user_data(event);
  lv_obj_t *control_light = create_control_light(data->name, data->screen);
  lv_screen_load_anim(control_light, LV_SCR_LOAD_ANIM_FADE_IN, 300, 0, false);
}

static void create_widgets(lv_obj_t *screen, widget_array *widgets) {

  for (int i = 0; i < widgets->length; i++) {
    lv_obj_t *btn = lv_btn_create(screen);
    lv_obj_set_style_bg_color(btn, lv_color_hex(0xff3864), 0);
    lv_obj_set_width(btn, lv_pct(25));
    lv_obj_set_height(btn, lv_pct(30));
    lv_obj_t *btn_label = lv_label_create(btn);
    lv_obj_set_align(btn_label, LV_ALIGN_CENTER);
    lv_label_set_text(btn_label, widgets->widgets[i].name);

    screen_data *data = malloc(sizeof(screen_data));
    data->name = widgets->widgets[i].name;
    data->screen = screen;
    lv_obj_add_event_cb(btn, show_control, LV_EVENT_CLICKED, data);
  }
}

void load_widgets_from_config(lv_obj_t *screen) {
  ESP_LOGI(PRIMARY_TAG, "button clicked");
  char *text = read_file("app_widgets.json");
  if (text == NULL) {
    ESP_LOGE(PRIMARY_TAG, "Failed to read file app_widgets.json");
    return;
  }
  ESP_LOGI(PRIMARY_TAG, "PARSING JSON");
  // print_widgets(text);
  curr_widgets = NULL;
  widget_array *array = get_widgets_array(text);
  curr_widgets = array;
  for (int i = 0; i < array->length; i++) {
    ESP_LOGI(PRIMARY_TAG, "ARRAY ELEMENT  %d:  NAME : %s TYPE : %s", i,
             array->widgets[i].name, array->widgets[i].type);
  }
  create_widgets(screen, curr_widgets);
  free(text);
}

lv_obj_t *create_primary_screen() {
  lv_obj_t *primary_screen = lv_obj_create(NULL);
  lv_obj_set_layout(primary_screen, LV_LAYOUT_FLEX);
  lv_obj_set_flex_flow(primary_screen, LV_FLEX_FLOW_ROW_WRAP);
  lv_obj_set_style_bg_color(primary_screen, lv_color_hex(0xf39c6b),
                            LV_STATE_DEFAULT);
  lv_obj_set_style_pad_top(primary_screen, 20, LV_STATE_DEFAULT);

  return primary_screen;
}