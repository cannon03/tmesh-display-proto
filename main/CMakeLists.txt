



idf_component_register(SRCS "main.c" "primary_screen/primary_screen.c" "settings_screen/settings_screen.c" "wifi_screen/wifi_screen.c" "control_light/control_light.c" "filesystem/filesystem.c" "utils/utils.c"
PRIV_REQUIRES spiffs esp_wifi              
INCLUDE_DIRS ".")
