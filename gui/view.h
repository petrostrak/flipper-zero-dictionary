// Mock view.h for simulation
#pragma once

#include "../furi.h"
#include "gui.h"
#include "../input/input.h"

// Functions
ViewPort* view_port_alloc();
void view_port_free(ViewPort* view_port);
void view_port_draw_callback_set(ViewPort* view_port, void (*callback)(Canvas*, void*), void* context);
void view_port_input_callback_set(ViewPort* view_port, void (*callback)(InputEvent*, void*), void* context);
void view_port_update(ViewPort* view_port);