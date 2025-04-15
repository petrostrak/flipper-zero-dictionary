// Mock elements.h for simulation
#pragma once

#include "../furi.h"
#include "gui.h"

// Forward declarations
typedef struct Canvas Canvas;

// Canvas drawing functions
void canvas_clear(Canvas* canvas);
void canvas_set_font(Canvas* canvas, int font);
void canvas_draw_str(Canvas* canvas, int x, int y, const char* str);
void canvas_draw_frame(Canvas* canvas, int x, int y, int width, int height);

// Font definitions
#define FontPrimary 1
#define FontSecondary 2