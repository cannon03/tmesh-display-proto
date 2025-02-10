#include <stdio.h>

#include "bsp/esp-bsp.h"

#include "core/lv_obj_event.h"
#include "display/lv_display.h"
#include "filesystem/filesystem.h"
#include "lv_api_map_v8.h"
#include "misc/lv_event.h"
#include "primary_screen/primary_screen.h"
#include "settings_screen/settings_screen.h"
#include "wifi_screen/wifi_screen.h"
lv_obj_t *primary_screen = NULL;
lv_obj_t *settings_screen = NULL;
lv_obj_t *curr_screen = NULL;

void primary_switch(lv_event_t *t) {
  lv_dir_t gesture = lv_indev_get_gesture_dir(lv_indev_get_act());
  ESP_LOGI("main", "Gesture detected %d", gesture);
  if (gesture == LV_DIR_LEFT) {
    bsp_display_lock(0);
    curr_screen = settings_screen;
    lv_scr_load_anim(curr_screen, LV_SCR_LOAD_ANIM_MOVE_LEFT, 300, 0, false);
    ESP_LOGI("main", "LOADED SETTINGS SCREEN");

    bsp_display_unlock();
  }
}

void settings_switch(lv_event_t *t) {
  lv_dir_t gesture = lv_indev_get_gesture_dir(lv_indev_get_act());
  ESP_LOGI("main", "Gesture detected %d", gesture);

  if (gesture == LV_DIR_RIGHT) {
    bsp_display_lock(0);
    curr_screen = primary_screen;
    lv_scr_load_anim(curr_screen, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 300, 0, false);
    ESP_LOGI("main", "LOADED PRIMARY SCREEN");
    bsp_display_unlock();
  }
}

void app_main(void) {

  init_spiffs();
  bsp_display_start();

  bsp_display_lock(0);
  primary_screen = create_primary_screen();
  settings_screen = create_settings_screen();
  curr_screen = primary_screen;
  lv_screen_load(curr_screen);
  bsp_display_unlock();
  load_widgets_from_config(primary_screen);

  lv_obj_add_event_cb(primary_screen, primary_switch, LV_EVENT_GESTURE, NULL);
  lv_obj_add_event_cb(settings_screen, settings_switch, LV_EVENT_GESTURE, NULL);
}
