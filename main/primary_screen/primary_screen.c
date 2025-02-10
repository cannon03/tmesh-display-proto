#include <stdio.h>
#include <string.h>

#include "esp_log.h"
#include "filesystem/filesystem.h"
#include "primary_screen.h"
#include "utils/utils.h"

static const char *PRIMARY_TAG = "primary_screen";

widget_array *curr_widgets = NULL;

// static bool spiffs_initialized = false;

// esp_err_t init_spiffs(void){
//     esp_vfs_spiffs_conf_t conf = {
//         .base_path = "/spiffs",
//         .partition_label = NULL,
//         .max_files = 5,
//         .format_if_mount_failed = true
//     };

//     esp_err_t ret = esp_vfs_spiffs_register(&conf);

//     if (ret != ESP_OK){
//         if (ret == ESP_FAIL){
//             ESP_LOGE(PRIMARY_TAG, "Failed to mount or format SPIFFS");
//         }else if (ret == ESP_ERR_NOT_FOUND){
//             ESP_LOGE(PRIMARY_TAG, "Failed to find SPIFFS partition");
//         }else{
//             ESP_LOGE(PRIMARY_TAG, "Failed to initialize SPIFFS (%s)",
//             esp_err_to_name(ret));
//         }
//         return ret;
//     }

//     size_t total = 0 ,used = 0;
//     ret = esp_spiffs_info(NULL, &total, &used);
//     if (ret != ESP_OK){
//         ESP_LOGE(PRIMARY_TAG, "Failed to get SPIFFS information (%s)",
//         esp_err_to_name(ret)); return ret;
//     }
//     ESP_LOGI(PRIMARY_TAG, "Partition size:%d, used size:%d", total, used);
//     spiffs_initialized = true;

//     return ESP_OK;

// }

// static void print_json_value(const char *buffer, jsmntok_t *t, int index, int
// indent) {
//     // Print indentation
//     for (int i = 0; i < indent; i++) {
//         ESP_LOGI(PRIMARY_TAG,"  ");
//     }

//     if (t[index].type == JSMN_PRIMITIVE || t[index].type == JSMN_STRING) {
//         ESP_LOGI(PRIMARY_TAG,"%.*s\n", t[index].end - t[index].start, buffer
//         + t[index].start);
//     } else if (t[index].type == JSMN_ARRAY) {
//         ESP_LOGI(PRIMARY_TAG,"[\n");
//         int array_size = t[index].size;
//         int current = index + 1;

//         for (int i = 0; i < array_size; i++) {
//             for (int j = 0; j < indent + 1; j++) ESP_LOGI(PRIMARY_TAG,"  ");
//             print_json_value(buffer, t, current, indent + 1);
//             current += 1;
//         }

//         for (int i = 0; i < indent; i++) ESP_LOGI(PRIMARY_TAG,"  ");
//         ESP_LOGI(PRIMARY_TAG,"]\n");
//     } else if (t[index].type == JSMN_OBJECT) {
//         ESP_LOGI(PRIMARY_TAG,"{\n");
//         int object_size = t[index].size;
//         int current = index + 1;

//         for (int i = 0; i < object_size; i++) {
//             for (int j = 0; j < indent + 1; j++) ESP_LOGI(PRIMARY_TAG,"  ");
//             ESP_LOGI(PRIMARY_TAG,"\"%.*s\": ",
//                    t[current].end - t[current].start,
//                    buffer + t[current].start);
//             print_json_value(buffer, t, current + 1, indent + 1);
//             current += 2;
//         }

//         for (int i = 0; i < indent; i++) ESP_LOGI(PRIMARY_TAG,"  ");
//         ESP_LOGI(PRIMARY_TAG,"}\n");
//     }
// }

// static char* get_json_tokens(char *buffer) {
//     jsmn_parser p;
//     jsmntok_t t[128]; /* We expect no more than 128 tokens */

//     jsmn_init(&p);
//     int r = jsmn_parse(&p, buffer, strlen(buffer), t, sizeof(t) /
//     sizeof(t[0]));

//     if (r < 0) {
//         ESP_LOGE("JSON", "Failed to parse JSON: %d", r);
//         return NULL;
//     }

//     /* Assume the top-level element is an object */
//     if (r < 1 || t[0].type != JSMN_OBJECT) {
//         ESP_LOGE("JSON", "Object expected");
//         return NULL;
//     }

//     ESP_LOGI(PRIMARY_TAG,"Parsed JSON:\n");
//     print_json_value(buffer, t, 0, 0);

//     return buffer;
// }

// static char *read_file(char *filename){

//     if (!spiffs_initialized){
//         // if (init_spiffs() != ESP_OK){
//         //     return NULL;
//         // }
//         // spiffs_initialized = true;
//         return NULL;
//     }

//     char filepath[64];
//     snprintf(filepath, sizeof(filepath), "/spiffs/%s", filename);
//     FILE *f= fopen(filepath, "r");
//     if (f == NULL){
//         ESP_LOGE(PRIMARY_TAG, "Failed to open file %s", filepath);
//         return NULL;
//     }

//     fseek(f, 0, SEEK_END);
//     long length = ftell(f);
//     fseek(f, 0, SEEK_SET);

//     char *buffer = (char*)malloc(length + 1);

//     if (buffer == NULL){
//         ESP_LOGE(PRIMARY_TAG, "Failed to allocate memory for file buffer");
//         fclose(f);
//         return NULL;
//     }

//     size_t read_length = fread(buffer,1,length,f);

//     fclose(f);
//     if (read_length != length){
//         ESP_LOGE(PRIMARY_TAG, "Failed to read file %s", filepath);
//         free(buffer);
//         return NULL;
//     }

//     buffer[length] = '\0';
//     return buffer;
// }

static void parse_button_cb(lv_event_t *e) {
  // ESP_LOGI(PRIMARY_TAG, "button clicked");
  // char *text = read_file("app_widgets.json");
  // if (text == NULL) {
  //   ESP_LOGE(PRIMARY_TAG, "Failed to read file app_widgets.json");
  //   return;
  // }
  // ESP_LOGI(PRIMARY_TAG, "PARSING JSON");
  // // print_widgets(text);
  // widget_array *array = get_widgets_array(text);
  // for (int i = 0; i < array->length; i++) {
  //   ESP_LOGI(PRIMARY_TAG, "ARRAY ELEMENT  %d:  NAME : %s TYPE : %s", i,
  //            array->widgets[i].name, array->widgets[i].type);
  // }
  // free(text);
}

static void create_widgets(lv_obj_t *screen, widget_array *widgets) {
  lv_obj_set_layout(screen, LV_LAYOUT_FLEX);
  lv_obj_set_flex_flow(screen, LV_FLEX_FLOW_ROW_WRAP);

  for (int i = 0; i < widgets->length; i++) {
    lv_obj_t *btn = lv_btn_create(screen);
    lv_obj_set_style_bg_color(btn, lv_color_hex(0xff3864), 0);
    lv_obj_set_width(btn, lv_pct(20));
    lv_obj_set_height(btn, lv_pct(20));
    lv_obj_t *btn_label = lv_label_create(btn);
    lv_obj_set_align(btn_label, LV_ALIGN_CENTER);
    lv_label_set_text(btn_label, widgets->widgets[i].name);
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

lv_obj_t *create() {
  lv_obj_t *primary_screen = lv_obj_create(NULL);
  lv_obj_set_style_bg_color(primary_screen, lv_color_hex(0xf39c6b),
                            LV_STATE_DEFAULT);
  lv_obj_set_style_pad_all(primary_screen, 20, LV_STATE_DEFAULT);

  return primary_screen;
}