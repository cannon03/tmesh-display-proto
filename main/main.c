#include <stdio.h>

#include "bsp/esp-bsp.h"

#include "primary_screen/primary_screen.h"

void app_main(void)
{

    init_spiffs();
    
    bsp_display_start();

    bsp_display_lock(0);

    lv_obj_t *primary_screen = create();
    lv_screen_load(primary_screen);


    bsp_display_unlock();

}
