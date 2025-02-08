#ifndef PRIMARY_SCREEN_H
#define PRIMARY_SCREEN_H
#include "lvgl.h"
#include "esp_spiffs.h"

lv_obj_t *create();
esp_err_t init_spiffs(void);
#endif /* PRIMARY_SCREEN_H */