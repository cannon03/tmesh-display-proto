#include "wifi_screen.h"
#include "core/lv_obj_event.h"
#include "esp_log.h"
#include "esp_mac.h"
#include "esp_wifi.h"
#include "lv_api_map_v8.h"
#include "misc/lv_event.h"
#include "widgets/keyboard/lv_keyboard.h"
#include <string.h>

static const char *WIFI_TAG = "WIFI_SCREEN";

typedef struct {
  lv_obj_t *ssid;
  lv_obj_t *password;
} user_data;

static void wifi_event_handler(void *arg, esp_event_base_t event_base,
                               int32_t event_id, void *event_data) {
  if (event_id == WIFI_EVENT_AP_STACONNECTED) {
    wifi_event_ap_staconnected_t *event =
        (wifi_event_ap_staconnected_t *)event_data;
    ESP_LOGI(WIFI_TAG, "station " MACSTR " join, AID=%d", MAC2STR(event->mac),
             event->aid);
  } else if (event_id == WIFI_EVENT_AP_STADISCONNECTED) {
    wifi_event_ap_stadisconnected_t *event =
        (wifi_event_ap_stadisconnected_t *)event_data;
    ESP_LOGI(WIFI_TAG, "station " MACSTR " leave, AID=%d, reason=%d",
             MAC2STR(event->mac), event->aid, event->reason);
  }
}

void wifi_init_softap(char *ssid, char *password) {
  ESP_ERROR_CHECK(esp_netif_init());
  ESP_ERROR_CHECK(esp_event_loop_create_default());
  esp_netif_create_default_wifi_ap();

  wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
  ESP_ERROR_CHECK(esp_wifi_init(&cfg));

  ESP_ERROR_CHECK(esp_event_handler_instance_register(
      WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL, NULL));

  wifi_config_t wifi_config = {0}; // Initialize to zero first

  strcpy((char *)wifi_config.ap.ssid, ssid);
  wifi_config.ap.ssid_len = strlen(ssid);
  strcpy((char *)wifi_config.ap.password, password);

  wifi_config.ap.channel = 1;
  wifi_config.ap.max_connection = 4;
#ifdef CONFIG_ESP_WIFI_SOFTAP_SAE_SUPPORT
  wifi_config.ap.authmode = WIFI_AUTH_WPA3_PSK;
  wifi_config.ap.sae_pwe_h2e = WPA3_SAE_PWE_BOTH;
#else /* CONFIG_ESP_WIFI_SOFTAP_SAE_SUPPORT */
  wifi_config.ap.authmode = WIFI_AUTH_WPA2_PSK;
#endif
  wifi_config.ap.pmf_cfg.required = true;
  if (strlen(password) == 0) {
    wifi_config.ap.authmode = WIFI_AUTH_OPEN;
  }

  ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
  ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &wifi_config));
  ESP_ERROR_CHECK(esp_wifi_start());

  ESP_LOGI(WIFI_TAG,
           "wifi_init_softap finished. SSID:%s password:%s channel:%d", ssid,
           password, 1);
}

void join_cb(lv_event_t *event) {
  user_data *data = (user_data *)lv_event_get_user_data(event);
  char *ssid = lv_textarea_get_text(data->ssid);
  char *password = lv_textarea_get_text(data->password);
  wifi_init_softap(ssid, password);
}

void request_keyboard(lv_event_t *e) {
  lv_textarea_t *text_area = (lv_textarea_t *)lv_event_get_user_data(e);
  lv_obj_t *keyboard = lv_keyboard_create(lv_scr_act());
  lv_keyboard_set_textarea(keyboard, (lv_obj_t *)text_area);
}

lv_obj_t *create_wifi_screen() {
  lv_obj_t *wifi_screen = lv_obj_create(NULL);
  lv_obj_set_layout(wifi_screen, LV_LAYOUT_FLEX);
  lv_obj_set_flex_flow(wifi_screen, LV_FLEX_COLUMN);
  lv_obj_set_style_bg_color(wifi_screen, lv_color_hex(0xf39c6b),
                            LV_STATE_DEFAULT);
  lv_obj_t *ssidLabel = lv_label_create(wifi_screen);
  lv_label_set_text(ssidLabel, "SSID");
  lv_obj_t *ssid = lv_textarea_create(wifi_screen);
  lv_obj_t *passwordLabel = lv_label_create(wifi_screen);
  lv_label_set_text(passwordLabel, "PASSWORD");
  lv_obj_t *password = lv_textarea_create(wifi_screen);

  lv_obj_t *init_btn = lv_btn_create(wifi_screen);
  lv_obj_t *init_label = lv_label_create(init_btn);
  lv_label_set_text(init_label, "JOIN");

  user_data data = {ssid, password};
  lv_obj_add_event_cb(init_btn, join_cb, LV_EVENT_CLICKED, &data);
  return wifi_screen;
}