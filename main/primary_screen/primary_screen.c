#include <stdio.h>
#include "primary_screen.h"

#include "esp_log.h"
#include "esp_spiffs.h"


static const char *TAG = "primary_screen";



static esp_err_t init_spiffs(void){
    esp_vfs_spiffs_conf_t conf = {
        .base_path = "/spiffs",
        .partition_label = NULL,
        .max_files = 5,
        .format_if_mount_failed = true
    };

    esp_err_t ret = esp_vfs_spiffs_register(&conf);

    if (ret != ESP_OK){
        if (ret == ESP_FAIL){
            ESP_LOGE(TAG, "Failed to mount or format SPIFFS");
        }else if (ret == ESP_ERR_NOT_FOUND){
            ESP_LOGE(TAG, "Failed to find SPIFFS partition");
        }else{
            ESP_LOGE(TAG, "Failed to initialize SPIFFS (%s)", esp_err_to_name(ret));
        }
        return ret;
    }

    size_t total = 0 ,used = 0;
    ret = esp_spiffs_info(NULL, &total, &used);
    if (ret != ESP_OK){
        ESP_LOGE(TAG, "Failed to get SPIFFS information (%s)", esp_err_to_name(ret));
        return ret;
    }
    ESP_LOGI(TAG, "Partition size:%d, used size:%d", total, used);
    return ESP_OK;

}


static char *read_file(char *filename){

    static bool spiffs_initialized = false;

    if (!spiffs_initialized){
        if (init_spiffs() != ESP_OK){
            return NULL;
        }
        spiffs_initialized = true;
    }

    char filepath[64];
    snprintf(filepath, sizeof(filepath), "/spiffs/%s", filename);
    FILE *f= fopen(filepath, "r");
    if (f == NULL){
        ESP_LOGE(TAG, "Failed to open file %s", filepath);
        return NULL;
    }

    fseek(f, 0, SEEK_END);
    long length = ftell(f);
    fseek(f, 0, SEEK_SET);

    char *buffer = (char*)malloc(length + 1);

    if (buffer == NULL){
        ESP_LOGE(TAG, "Failed to allocate memory for file buffer");
        fclose(f);
        return NULL;
    }

    size_t read_length = fread(buffer,1,length,f);

    if (read_length != length){
        ESP_LOGE(TAG, "Failed to read file %s", filepath);
        fclose(f);
        free(buffer);
        return NULL;
    }

    buffer[length] = '\0';
    return buffer;

}



lv_obj_t *create(){
    lv_obj_t *primary_screen = lv_obj_create(NULL);
    lv_obj_t *label = lv_label_create(primary_screen);
    char *text = read_file("app_widgets.json");
    printf("Text: %s\n", text);
    lv_obj_set_align(label, LV_ALIGN_CENTER);
    lv_label_set_text(label, text);
    free(text);
    return primary_screen;
}