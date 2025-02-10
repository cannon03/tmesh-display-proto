#ifndef TMESH_FILESYSTEM_H
#define TMESH_FILESYSTEM_H
#include "esp_log.h"
#include "esp_spiffs.h"
#include <stdbool.h>

bool is_spiffs_initialized();
esp_err_t init_spiffs(void);
char *read_file(char *filename);
#endif