#include <stdio.h>
#include <string.h>

#include "filesystem/filesystem.h"
#include "primary_screen.h"
#include "utils/utils.h"

#include "bsp/esp32_s3_lcd_ev_board.h"
#include "esp_log.h"

static const char *PRIMARY_TAG = "primary_screen";

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
  ESP_LOGI(PRIMARY_TAG, "button clicked");
  char *text = read_file("app_widgets.json");
  if (text == NULL) {
    ESP_LOGE(PRIMARY_TAG, "Failed to read file app_widgets.json");
    return;
  }
  ESP_LOGI(PRIMARY_TAG, "PARSING JSON");
  print_widgets(text);
  free(text);
}

lv_obj_t *create() {
  lv_obj_t *primary_screen = lv_obj_create(NULL);

  // Create parse button
  lv_obj_t *parse_btn = lv_btn_create(primary_screen);
  lv_obj_add_event_cb(parse_btn, parse_button_cb, LV_EVENT_CLICKED, NULL);
  lv_obj_align(parse_btn, LV_ALIGN_TOP_MID, 0, 10);

  // Add label to button
  lv_obj_t *btn_label = lv_label_create(parse_btn);
  lv_label_set_text(btn_label, "Parse JSON");
  lv_obj_center(btn_label);
  return primary_screen;
}