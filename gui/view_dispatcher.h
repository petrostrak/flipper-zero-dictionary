// Mock view_dispatcher.h for simulation
#pragma once

#include "../furi.h"
#include "gui.h"

// Forward declarations
typedef struct ViewDispatcher ViewDispatcher;

// Functions
ViewDispatcher* view_dispatcher_alloc();
void view_dispatcher_free(ViewDispatcher* view_dispatcher);
void view_dispatcher_run(ViewDispatcher* view_dispatcher);
void view_dispatcher_stop(ViewDispatcher* view_dispatcher);