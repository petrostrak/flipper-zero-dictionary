#pragma once

#include "furi.h"
#include "gui/gui.h"
#include "gui/view_dispatcher.h"
#include "gui/scene_manager.h"
#include "notification/notification_messages.h"

// Define the main dictionary application structure
typedef struct {
    Gui* gui;
    ViewDispatcher* view_dispatcher;
    ViewPort* view_port;
    FuriMessageQueue* event_queue;
    NotificationApp* notifications;

    // Search state
    char search_term[32];
    uint8_t search_term_length;
    bool showing_definition;

    // Navigation state
    uint32_t current_word_index;
    uint32_t scroll_position;
    bool is_searching;
    
    // Favorites functionality
    bool showing_favorites;        // Flag to show if in favorites view
    uint32_t favorites[50];        // Store indices of favorite words (max 50)
    uint8_t favorites_count;       // Number of favorites saved
    uint8_t current_favorite_index; // Current position in favorites list
    
    // Search results functionality
    bool showing_search_results;   // Flag to show search results
    uint32_t* search_results;      // Array of indices for search results
    uint32_t search_results_count; // Number of search results
    uint32_t current_search_index; // Current position in search results
    
    // Translation functionality
    bool show_translation;         // Flag to toggle between definition/translation
} DictionaryApp;

// Main entry point for the application
int32_t dictionary_app(void* p);
