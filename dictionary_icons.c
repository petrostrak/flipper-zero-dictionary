#include "gui/icon.h"

const uint8_t dictionary_icon_data[] = {
    // 10x10 px dictionary icon for Flipper Zero
    0x0F, 0xF0, // ####
    0x3C, 0xF0, // ####  ##
    0x30, 0xC0, // ##    ##
    0x3F, 0xC0, // ########
    0x3F, 0xC0, // ########
    0x3F, 0xC0, // ########
    0x3F, 0xC0, // ########
    0x30, 0xC0, // ##    ##
    0x3C, 0xF0, // ####  ##
    0x0F, 0xF0, // ####
};

const Icon dictionary_10px = {
    .width = 10,
    .height = 10,
    .frame_count = 1,
    .frame_rate = 0,
    .frames = dictionary_icon_data
};
