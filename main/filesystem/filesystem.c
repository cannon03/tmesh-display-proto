#include "filesystem.h"

static bool spiffs_initialized = false;

bool is_spiffs_initialized() { return spiffs_initialized; }

const char *FILESYSTEM_TAG = "FILESYSTEM";

esp_err_t init_spiffs(void) { // Initialize SPI NOR under /spiffs

  if (is_spiffs_initialized())
    return ESP_OK;

  esp_vfs_spiffs_conf_t conf = {.base_path = "/spiffs",
                                .partition_label = NULL,
                                .max_files = 5,
                                .format_if_mount_failed = true};

  esp_err_t ret = esp_vfs_spiffs_register(&conf);

  if (ret != ESP_OK) {
    if (ret == ESP_FAIL) {
      ESP_LOGE(FILESYSTEM_TAG, "Failed to mount or format SPIFFS");
    } else if (ret == ESP_ERR_NOT_FOUND) {
      ESP_LOGE(FILESYSTEM_TAG, "Failed to find SPIFFS partition");
    } else {
      ESP_LOGE(FILESYSTEM_TAG, "Failed to initialize SPIFFS (%s)",
               esp_err_to_name(ret));
    }
    return ret;
  }

  size_t total = 0, used = 0;
  ret = esp_spiffs_info(NULL, &total, &used);
  if (ret != ESP_OK) {
    ESP_LOGE(FILESYSTEM_TAG, "Failed to get SPIFFS information (%s)",
             esp_err_to_name(ret));
    return ret;
  }
  ESP_LOGI(FILESYSTEM_TAG, "Partition size:%d, used size:%d", total, used);
  spiffs_initialized = true;

  return ESP_OK;
}

char *read_file(char *filename) {

  if (!spiffs_initialized) {
    return NULL;
  }

  char filepath[64];
  snprintf(filepath, sizeof(filepath), "/spiffs/%s", filename);
  FILE *f = fopen(filepath, "r");
  if (f == NULL) {
    ESP_LOGE(FILESYSTEM_TAG, "Failed to open file %s", filepath);
    return NULL;
  }

  fseek(f, 0, SEEK_END);
  long length = ftell(f);
  fseek(f, 0, SEEK_SET);

  char *buffer = (char *)malloc(length + 1);

  if (buffer == NULL) {
    ESP_LOGE(FILESYSTEM_TAG, "Failed to allocate memory for file buffer");
    fclose(f);
    return NULL;
  }

  size_t read_length = fread(buffer, 1, length, f);

  fclose(f);
  if (read_length != length) {
    ESP_LOGE(FILESYSTEM_TAG, "Failed to read file %s", filepath);
    free(buffer);
    return NULL;
  }

  buffer[length] = '\0';
  return buffer;
}
