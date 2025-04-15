// Mock scene_manager.h for simulation
#pragma once

#include "../furi.h"

// Forward declarations
typedef struct SceneManager SceneManager;

// Functions
SceneManager* scene_manager_alloc();
void scene_manager_free(SceneManager* scene_manager);