#ifndef TMESH_UTILS_H
#define TMESH_UTILS_H

#include "esp_log.h"
#define JSMN_STATIC
#include "jsmn/jsmn.h"
#include <stdbool.h>
#include <string.h>

typedef struct {
  int length;
  char *start;
} tmesh_string_ref;

typedef enum {
  TMESH_WIDGET_LIGHT,
  TMESH_WIDGET_FAN,
  TMESH_WIDGET_UNKNOWN
} tmesh_widget_type;

typedef struct {
  char *name;
  char *type;
} tmesh_widget;

typedef struct {
  tmesh_widget *widgets;
  size_t length;
} widget_array;

widget_array *get_widgets_array(char *json_string);
void print_widgets(char *json_string);
#endif