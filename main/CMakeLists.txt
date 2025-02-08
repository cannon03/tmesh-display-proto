# file(GLOB SRC_FILES "*.c" "primary_screen/*.c") 


idf_component_register(SRCS "main.c" "primary_screen/primary_screen.c"
PRIV_REQUIRES spiffs                    
INCLUDE_DIRS ".")
