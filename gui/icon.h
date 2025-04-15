// Mock icon.h for simulation
#pragma once

#include <stdint.h>

// Basic icon structure for the Flipper Zero UI
typedef struct {
    const uint8_t width;
    const uint8_t height;
    const uint8_t frame_count;
    const uint8_t frame_rate;
    const uint8_t* const frames;
} Icon;