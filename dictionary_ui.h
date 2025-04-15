#pragma once

#include "gui/view.h"
#include "input/input.h"

// Forward declaration - should match the actual struct in dictionary_app.h
struct DictionaryApp;

// UI drawing callback
void dictionary_ui_draw_callback(Canvas* canvas, void* context);

// UI input handling callback
void dictionary_ui_input_callback(InputEvent* input_event, void* ctx);

// Helper functions
int dictionary_ui_count_lines(const char* text);
