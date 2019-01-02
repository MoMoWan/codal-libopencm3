#ifndef UF2_CFG_H
#define UF2_CFG_H

//  Define UF2 constants
#define UF2_VERSION     "1.1.3"  //  Previously "1.00".  This is checked by MakeCode.
#define PRODUCT_NAME    "STM32BLUEPILL"
#define BOARD_ID        "STM32F103C8-BluePill-v0"  //   CPU type - board type - board revision
#define INDEX_URL       "https://visualbluepill.github.io"
#define UF2_NUM_BLOCKS  8000
#define VOLUME_LABEL    "BLUEPILL"

//  Response for the HF2_CMD_INFO request, also contents of the INFO_UF2.TXT file.
#define UF2_INFO_TEXT \
    "UF2 Bootloader v"  UF2_VERSION " SFA\r\n" \
    "Model: "           PRODUCT_NAME "\r\n" \
    "Board-ID: "        BOARD_ID "\r\n"

// where the UF2 files are allowed to write data - we allow MBR, since it seems part of the softdevice .hex file
#define USER_FLASH_START    (uint32_t)(APP_BASE_ADDRESS)
#define USER_FLASH_END      (0x08000000 + FLASH_SIZE_OVERRIDE)  

#endif  //  UF2_CFG_H
