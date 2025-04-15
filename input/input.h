// Mock input.h for simulation
#pragma once

#include "../furi.h"

// Input event structure
typedef struct {
    int type;
    int key;
} InputEvent;

// Constants for input handling
#define InputTypePress 1
#define InputTypeRelease 2
#define InputKeyUp 1
#define InputKeyDown 2
#define InputKeyRight 3
#define InputKeyLeft 4
#define InputKeyOk 5
#define InputKeyBack 6
