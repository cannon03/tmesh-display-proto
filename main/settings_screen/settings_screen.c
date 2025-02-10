#include "settings_screen.h"
#include "core/lv_obj_pos.h"
#include "core/lv_obj_style_gen.h"

lv_obj_t *create_settings_screen() {
  lv_obj_t *settings_screen = lv_obj_create(NULL);
  lv_obj_set_flex_flow(settings_screen, LV_LAYOUT_FLEX);
  lv_obj_set_flex_flow(settings_screen, LV_FLEX_FLOW_COLUMN);
  lv_obj_set_style_bg_color(settings_screen, lv_color_hex(0xf39c6b),
                            LV_STATE_DEFAULT);
  lv_obj_set_style_pad_all(settings_screen, 20, LV_STATE_DEFAULT);
  lv_obj_set_style_pad_column(settings_screen, 10, LV_STATE_DEFAULT);
  lv_obj_t *ota_menu_item = lv_obj_create(settings_screen);
  lv_obj_set_style_outline_width(ota_menu_item, 4, LV_STATE_DEFAULT);
  lv_obj_set_width(ota_menu_item, lv_pct(90));
  lv_obj_set_height(ota_menu_item, lv_pct(10));
  lv_obj_t *label = lv_label_create(ota_menu_item);
  lv_label_set_text(label, "OTA UPDATE >>>");
  lv_obj_set_align(label, LV_ALIGN_CENTER);

  lv_obj_t *refresh_menu_itm = lv_obj_create(settings_screen);
  lv_obj_set_style_outline_width(refresh_menu_itm, 4, LV_STATE_DEFAULT);
  lv_obj_set_width(refresh_menu_itm, lv_pct(90));
  lv_obj_set_height(refresh_menu_itm, lv_pct(10));
  lv_obj_t *refresh_label = lv_label_create(refresh_menu_itm);
  lv_label_set_text(refresh_label, "REFRESH SCREEN");
  lv_obj_set_align(refresh_label, LV_ALIGN_CENTER);

  return settings_screen;
}