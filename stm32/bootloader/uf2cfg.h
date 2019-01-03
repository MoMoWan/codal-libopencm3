#ifndef UF2_CFG_H
#define UF2_CFG_H

//  Define UF2 constants
#define UF2_VERSION     "1.1.3"  //  Previously "1.00".  This is checked by MakeCode.
#define PRODUCT_NAME    "STM32BLUEPILL"
#define BOARD_ID        "STM32F103C8-BluePill-v0"  //   CPU type - board type - board revision
#define INDEX_URL       "https://visualbluepill.github.io"
#define UF2_NUM_BLOCKS  8000
#define VOLUME_LABEL    "BLUEPILL"

//  Identification of this device as the response for the MakeCode HF2_CMD_INFO request. Also used as contents of the INFO_UF2.TXT file.
#define UF2_INFO_TEXT \
    "UF2 Bootloader v"  UF2_VERSION     " SFA\r\n" \
    "Model: "           PRODUCT_NAME    "\r\n" \
    "Board-ID: "        BOARD_ID        "\r\n"

//  Define the addresses where MakeCode is allowed to write to flash.  Should be in high ROM between _text (e.g. 8006800) and end of ROM (e.g. 8010000).
//  Don't allow writing into the Bootloader at low ROM, between _boot_text (e.g. 8000000) and _boot_etext (e.g. 8004ad0).
#define USER_FLASH_START    ((uint32_t) APP_BASE_ADDRESS)
#define USER_FLASH_END      (0x08000000 + FLASH_SIZE_OVERRIDE)  

#endif  //  UF2_CFG_H
