#include "control_light.h"
#include "core/lv_obj.h"
#include "core/lv_obj_pos.h"
#include "display/lv_display.h"
#include "esp_log.h"
#include "misc/lv_area.h"
#include "misc/lv_event.h"
#include "misc/lv_types.h"
#include "widgets/switch/lv_switch.h"

static const char *CONTROL_LIGHT_TAG = "CONTROL_LIGHT";

void back(lv_event_t *e) {
  lv_obj_t *previous = (lv_obj_t *)lv_event_get_user_data(e);
  ESP_LOGI(CONTROL_LIGHT_TAG, "back clicked");
  lv_screen_load_anim(previous, LV_SCR_LOAD_ANIM_FADE_OUT, 300, 0, true);
  ESP_LOGI(CONTROL_LIGHT_TAG, "LOADED primary screen");
}

lv_obj_t *create_control_light(char *name, lv_obj_t *previous) {
  lv_obj_t *control_light = lv_obj_create(NULL);
  lv_obj_set_style_pad_all(control_light, 20, LV_STATE_DEFAULT);
  lv_obj_t *back_btn = lv_btn_create(control_light);
  lv_obj_t *back_label = lv_label_create(back_btn);
  lv_label_set_text(back_label, "< BACK");
  lv_obj_t *light_label = lv_label_create(control_light);
  lv_label_set_text(light_label, name);
  lv_obj_t *light_switch = lv_switch_create(control_light);
  lv_obj_set_width(light_switch, lv_pct(70));
  lv_obj_set_height(light_switch, lv_pct(30));
  lv_obj_align(light_switch, LV_ALIGN_CENTER, 0, 0);
  lv_obj_set_align(back_btn, LV_ALIGN_TOP_LEFT);
  lv_obj_set_align(back_label, LV_ALIGN_CENTER);
  lv_obj_set_align(light_label, LV_ALIGN_TOP_MID);
  lv_obj_add_event_cb(back_btn, back, LV_EVENT_CLICKED, previous);
  return control_light;
}