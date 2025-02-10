



idf_component_register(SRCS "main.c" "primary_screen/primary_screen.c" "filesystem/filesystem.c" "utils/utils.c"
PRIV_REQUIRES spiffs    
REQUIRES json            
INCLUDE_DIRS ".")
