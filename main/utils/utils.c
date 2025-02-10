#include "utils.h"
#include "esp_log.h"
#include "jsmn/jsmn.h"
#include <stdio.h>
#include <string.h>

#define MAX_TOKENS 128
const char *UTILS_TAG = "UTILS";

static int jsoneq(const char *json, jsmntok_t *tok, const char *s) {
  if (tok->type == JSMN_STRING && (int)strlen(s) == tok->end - tok->start &&
      strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
    return 0;
  }
  return -1;
}

void parse_json(char *buffer) {
  jsmn_parser p;
  jsmntok_t t[128];
  /* We expect no more than 128 tokens */

  jsmn_init(&p);
  int r = jsmn_parse(&p, buffer, strlen(buffer), t, sizeof(t) / sizeof(t[0]));

  if (r < 0) {
    ESP_LOGE("JSON", "Failed to parse JSON: %d", r);
    // return NULL;
    return;
  }

  /* Assume the top-level element is an object */
  if (r < 1 || t[0].type != JSMN_OBJECT) {
    ESP_LOGE("JSON", "Object expected");
    // return NULL;
    return;
  }

  int i = 1;
  if (jsoneq(buffer, &t[i], "widgets") != 0) {
    return;
  }

  ESP_LOGI(UTILS_TAG, "KEY `widgets` found");
  i++;

  if (t[i].type != JSMN_ARRAY || i > r) {
    return;
  }

  ESP_LOGI(UTILS_TAG, "Parsing widgets Array");

  i++;

  tmesh_widget widgets[100];
  size_t widget_count = 0;

  while (i < r) {
    if (t[i].type == JSMN_OBJECT) {
      i++;
      ESP_LOGI(UTILS_TAG, "Object found");
      tmesh_widget w;
      w.name = NULL;
      w.type = NULL;
      widgets[widget_count] = w;
      widget_count++;
      continue;
    }
    // // print key
    // ESP_LOGI(UTILS_TAG, "KEY: \"%.*s\": ", t[i].end - t[i].start,
    //          buffer + t[i].start);
    // // print value
    // ESP_LOGI(UTILS_TAG, "VALUE :\"%.*s\": ", t[i + 1].end - t[i + 1].start,
    //          buffer + t[i + 1].start);

    size_t count = widget_count - 1;
    if (jsoneq(buffer, &t[i], "name") == 0) {
      tmesh_string_ref ref;
      ref.length = t[i + 1].end - t[i + 1].start;
      ref.start = buffer + t[i + 1].start;

      widgets[count].name = malloc(ref.length + 1);
      // ESP_LOGI(UTILS_TAG, "Allocated name buffer of size %d", ref.length +
      // 1);
      memcpy(widgets[count].name, ref.start, ref.length);
      widgets[count].name[ref.length] = '\0';
      // ESP_LOGI(UTILS_TAG, "Copied name: '%.*s' length=%d", ref.length,
      //          ref.start, ref.length);
      // ESP_LOGI(UTILS_TAG, "Result name: '%s'", widgets[count].name);

      // ESP_LOGI(UTILS_TAG, "CREATED AT INDEX %d", count);
    } else if (jsoneq(buffer, &t[i], "type") == 0) {
      tmesh_string_ref ref;
      ref.length = t[i + 1].end - t[i + 1].start;
      ref.start = buffer + t[i + 1].start;
      widgets[count].type = malloc(ref.length + 1);
      // ESP_LOGI(UTILS_TAG, "Allocated type buffer of size %d", ref.length +
      // 1);
      memcpy(widgets[count].type, ref.start, ref.length);
      widgets[count].type[ref.length] = '\0';
      // ESP_LOGI(UTILS_TAG, "Copied type: '%.*s' length=%d", ref.length,
      //          ref.start, ref.length);
      // ESP_LOGI(UTILS_TAG, "Result type: '%s'", widgets[count].type);
      // ESP_LOGI(UTILS_TAG, "CREATED AT INDEX %d", count);
    }
    i += 2;
  }

  for (size_t i = 0; i < 5; i++) {
    ESP_LOGI(UTILS_TAG, "NAME : %s", widgets[i].name);
    ESP_LOGI(UTILS_TAG, "TYPE : %s", widgets[i].type);
  }
}

void print_widgets(char *json_string) {
  parse_json(json_string);
  ESP_LOGI(UTILS_TAG, "JSON Parsed Sucessfully");
  // get_widgets(tokens, json_string);
}