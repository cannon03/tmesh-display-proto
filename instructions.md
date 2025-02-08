# Building a project on esp32s3-lcd-ev:

1.  Add dependency for esp32s3-lcd-ev bsp:
    ``` 
    idf.py add-dependency "espressif/esp32_s3_lcd_ev_board^3.0.0" 
    ```
2.  Enable psram and config:

    ```
    #
    # ESP PSRAM
    #
    CONFIG_SPIRAM=y

    #
    # SPI RAM config
    #
    # CONFIG_SPIRAM_MODE_QUAD is not set
    CONFIG_SPIRAM_MODE_OCT=y
    CONFIG_SPIRAM_TYPE_AUTO=y
    # CONFIG_SPIRAM_TYPE_ESPPSRAM64 is not set
    CONFIG_SPIRAM_CLK_IO=30
    CONFIG_SPIRAM_CS_IO=26
    # CONFIG_SPIRAM_XIP_FROM_PSRAM is not set
    CONFIG_SPIRAM_FETCH_INSTRUCTIONS=y
    CONFIG_SPIRAM_RODATA=y
    CONFIG_SPIRAM_SPEED_80M=y
    # CONFIG_SPIRAM_SPEED_40M is not set
    CONFIG_SPIRAM_SPEED=80
    # CONFIG_SPIRAM_ECC_ENABLE is not set
    CONFIG_SPIRAM_BOOT_INIT=y
    # CONFIG_SPIRAM_IGNORE_NOTFOUND is not set
    # CONFIG_SPIRAM_USE_MEMMAP is not set
    # CONFIG_SPIRAM_USE_CAPS_ALLOC is not set
    CONFIG_SPIRAM_USE_MALLOC=y
    CONFIG_SPIRAM_MEMTEST=y
    CONFIG_SPIRAM_MALLOC_ALWAYSINTERNAL=16384
    # CONFIG_SPIRAM_TRY_ALLOCATE_WIFI_LWIP is not set
    CONFIG_SPIRAM_MALLOC_RESERVE_INTERNAL=32768
    # CONFIG_SPIRAM_ALLOW_BSS_SEG_EXTERNAL_MEMORY is not set
    # CONFIG_SPIRAM_ALLOW_NOINIT_SEG_EXTERNAL_MEMORY is not set
    # end of SPI RAM config
    # end of ESP PSRAM
    ```
3.  Disable codec audio backward compatability:

    ```
    #
    # Audio Codec Device Configuration
    #
    # CONFIG_CODEC_I2C_BACKWARD_COMPATIBLE is not set
    ```
4.  For enabling spiffs:

    Create partition table :
       ```  
       # Example
       # Name,Type, SubType, Offset,  Size, Flags
       # Note: if you have increased the bootloader size, make sure to update the offsets to avoid overlap
       nvs,      data, nvs,     0x9000,  0x6000,
       phy_init, data, phy,     0xf000,  0x1000,
       factory,  app,  factory, 0x10000, 1M,
       storage,  data, spiffs,  ,        0xF0000,
       ```
       ```
       # main/CMakeLists.txt

       idf_component_register(SRCS "main.c"
       PRIV_REQUIRES spiffs          #Add REQUIRES / PRIV_REQUIRES          
       INCLUDE_DIRS ".")
       ```
