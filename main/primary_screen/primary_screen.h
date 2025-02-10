#ifndef PRIMARY_SCREEN_H
#define PRIMARY_SCREEN_H
#include "esp_spiffs.h"
#include "lvgl.h"

lv_obj_t *create();
void load_widgets_from_config(lv_obj_t *screen);
#endif /* PRIMARY_SCREEN_H */