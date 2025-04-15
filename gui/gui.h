// Mock gui.h for simulation
#pragma once

#include "../furi.h"
#include "../input/input.h"

// Forward declarations
typedef struct Gui Gui;
typedef struct ViewPort ViewPort;
typedef struct Canvas Canvas;

// Constants
#define GuiLayerFullscreen 0

// Functions
ViewPort* view_port_alloc();
void view_port_free(ViewPort* view_port);
void view_port_draw_callback_set(ViewPort* view_port, void (*callback)(Canvas*, void*), void* context);
void view_port_input_callback_set(ViewPort* view_port, void (*callback)(InputEvent*, void*), void* context);
void view_port_update(ViewPort* view_port);
void gui_add_view_port(Gui* gui, ViewPort* view_port, int layer);
void gui_remove_view_port(Gui* gui, ViewPort* view_port);
