#include "dictionary_app.h"
#include "dictionary_i.h"
#include "dictionary_data.h"
#include "dictionary_ui.h"

#include "furi.h"
#include "gui/elements.h"
#include "input/input.h"
#include "notification/notification_messages.h"

// Standard C libraries
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Initialize the dictionary application
static bool dictionary_app_init(DictionaryApp* app) {
    // Initialize the event queue
    app->event_queue = furi_message_queue_alloc(8, sizeof(InputEvent));

    // Initialize GUI
    app->gui = furi_record_open(RECORD_GUI);
    app->view_port = view_port_alloc();
    view_port_draw_callback_set(app->view_port, dictionary_ui_draw_callback, app);
    view_port_input_callback_set(app->view_port, dictionary_ui_input_callback, app);
    gui_add_view_port(app->gui, app->view_port, GuiLayerFullscreen);

    // Initialize notifications
    app->notifications = furi_record_open(RECORD_NOTIFICATION);

    // Initialize application state
    app->search_term_length = 0;
    app->search_term[0] = '\0';
    app->current_word_index = 0;
    app->scroll_position = 0;
    app->showing_definition = false;
    app->is_searching = false;
    
    // Initialize favorites
    app->favorites_count = 0;
    app->showing_favorites = false;
    app->current_favorite_index = 0;
    memset(app->favorites, 0, sizeof(app->favorites));
    
    // Initialize search results
    app->showing_search_results = false;
    app->search_results = NULL;
    app->search_results_count = 0;
    app->current_search_index = 0;
    
    // Initialize translation mode
    app->show_translation = false;

    // Initialize dictionary data
    dictionary_data_init();

    return true;
}

// Helper function to check if a word is a favorite
static bool is_word_favorite(DictionaryApp* app, uint32_t word_index) {
    for(uint8_t i = 0; i < app->favorites_count; i++) {
        if(app->favorites[i] == word_index) {
            return true;
        }
    }
    return false;
}

// Helper function to add a word to favorites
static bool add_to_favorites(DictionaryApp* app, uint32_t word_index) {
    // Check if word is already a favorite
    if(is_word_favorite(app, word_index)) {
        return false; // Already a favorite
    }
    
    // Check if favorites list is full
    if(app->favorites_count >= 50) {
        return false; // Favorites list is full
    }
    
    // Add word to favorites
    app->favorites[app->favorites_count] = word_index;
    app->favorites_count++;
    return true;
}

// Helper function to remove a word from favorites
static bool remove_from_favorites(DictionaryApp* app, uint32_t word_index) {
    // Find the word in favorites
    for(uint8_t i = 0; i < app->favorites_count; i++) {
        if(app->favorites[i] == word_index) {
            // Remove by shifting all elements after it
            for(uint8_t j = i; j < app->favorites_count - 1; j++) {
                app->favorites[j] = app->favorites[j + 1];
            }
            app->favorites_count--;
            
            // Adjust current_favorite_index if needed
            if(app->current_favorite_index >= app->favorites_count && app->favorites_count > 0) {
                app->current_favorite_index = app->favorites_count - 1;
            }
            
            return true; // Successfully removed
        }
    }
    return false; // Word was not in favorites
}

// Free resources used by the application
static void dictionary_app_free(DictionaryApp* app) {
    // Free GUI resources
    gui_remove_view_port(app->gui, app->view_port);
    view_port_free(app->view_port);
    furi_record_close(RECORD_GUI);

    // Free notification
    furi_record_close(RECORD_NOTIFICATION);

    // Free event queue
    furi_message_queue_free(app->event_queue);
    
    // Free search results if allocated
    if(app->search_results != NULL) {
        free(app->search_results);
        app->search_results = NULL;
    }

    // Clean up dictionary data
    dictionary_data_free();
}

// Main application entry point
int32_t dictionary_app(void* p) {
    UNUSED(p);
    
    // Allocate application state
    DictionaryApp* app = malloc(sizeof(DictionaryApp));
    
    // Initialize application
    if(!dictionary_app_init(app)) {
        dictionary_app_free(app);
        free(app);
        return 255;
    }

    // Notify user that application started
    notification_message(app->notifications, &sequence_display_backlight_on);

    // Main event loop
    InputEvent event;
    bool running = true;
    
    while(running) {
        // Get next input event
        FuriStatus status = furi_message_queue_get(app->event_queue, &event, 100);
        
        if(status == FuriStatusError) {
            // Exit if there was an error
            running = false;
            printf("Application exiting due to error...\n");
            continue;
        }
        
        // Only process events if we got a valid input (not a timeout)
        if(status == FuriStatusOk) {
            // Debug log for input events
            printf("Got input: type=%d, key=%d\n", event.type, event.key);
            
            // Process input based on current state
            if(app->is_searching) {
                // Process input for search mode
                if(event.type == InputTypePress) {
                    if(event.key == InputKeyOk) {
                        app->is_searching = false;
                        if(app->search_term_length > 0) {
                            // Free previous search results if any
                            if(app->search_results != NULL) {
                                free(app->search_results);
                                app->search_results = NULL;
                            }
                            
                            // Perform search with the entered term as prefix
                            app->search_results_count = dictionary_data_find_words_with_prefix(
                                app->search_term, &app->search_results);
                                
                            if(app->search_results_count > 0) {
                                // Show search results
                                app->showing_search_results = true;
                                app->current_search_index = 0;
                            } else {
                                // No results found, show a message
                                app->showing_definition = true;
                                strcpy(app->search_term, "No results found");
                                app->scroll_position = 0;
                            }
                        }
                    } else if(event.key == InputKeyBack) {
                        if(app->search_term_length > 0) {
                            app->search_term_length--;
                            app->search_term[app->search_term_length] = '\0';
                        } else {
                            app->is_searching = false;
                        }
                    } else if(event.key == InputKeyUp) {
                        // Navigate through alphabet - go to previous letter
                        if(app->search_term_length > 0) {
                            char current = app->search_term[app->search_term_length - 1];
                            if(current > 'a') {
                                app->search_term[app->search_term_length - 1]--;
                            } else {
                                app->search_term[app->search_term_length - 1] = 'z';
                            }
                        }
                    } else if(event.key == InputKeyDown) {
                        // Navigate through alphabet - go to next letter
                        if(app->search_term_length > 0) {
                            char current = app->search_term[app->search_term_length - 1];
                            if(current < 'z') {
                                app->search_term[app->search_term_length - 1]++;
                            } else {
                                app->search_term[app->search_term_length - 1] = 'a';
                            }
                        }
                    } else if(event.key == InputKeyRight) {
                        // Add new letter
                        if(app->search_term_length < 31) {
                            app->search_term[app->search_term_length] = 'a';
                            app->search_term_length++;
                            app->search_term[app->search_term_length] = '\0';
                        }
                    } else if(event.key == InputKeyLeft) {
                        // Remove last letter
                        if(app->search_term_length > 0) {
                            app->search_term_length--;
                            app->search_term[app->search_term_length] = '\0';
                        }
                    }
                }
            } else if(app->showing_definition) {
                // Process input for definition view
                if(event.type == InputTypePress) {
                    if(event.key == InputKeyOk || event.key == InputKeyBack) {
                        app->showing_definition = false;
                    } else if(event.key == InputKeyLeft) {
                        // Toggle between definition and translation
                        app->show_translation = !app->show_translation;
                        app->scroll_position = 0; // Reset scroll when switching modes
                    } else if(event.key == InputKeyUp) {
                        if(app->scroll_position > 0) {
                            app->scroll_position--;
                        }
                    } else if(event.key == InputKeyDown) {
                        // Get the current word and its text (definition or translation)
                        const char* word = dictionary_data_get_word(app->current_word_index);
                        const char* text;
                        
                        if(app->show_translation) {
                            text = dictionary_data_get_translation_by_index(app->current_word_index);
                        } else {
                            text = dictionary_data_get_definition(word);
                        }
                        
                        int lines = dictionary_ui_count_lines(text);
                        int max_lines = 4; // Maximum visible lines on screen
                        
                        if((int)app->scroll_position < lines - max_lines) {
                            app->scroll_position++;
                        }
                    }
                }
            } else if(app->showing_search_results) {
                // Search results navigation
                if(event.type == InputTypePress) {
                    if(event.key == InputKeyOk) {
                        if(app->search_results_count > 0) {
                            // Show definition of the selected search result
                            app->current_word_index = app->search_results[app->current_search_index];
                            
                            // Get the word from index and copy it to search_term for showing definition
                            const char* word = dictionary_data_get_word(app->current_word_index);
                            strncpy(app->search_term, word, sizeof(app->search_term) - 1);
                            app->search_term[sizeof(app->search_term) - 1] = '\0';
                            app->search_term_length = strlen(app->search_term);
                            
                            // Show definition view
                            app->showing_search_results = false;
                            app->showing_definition = true;
                            app->scroll_position = 0;
                        }
                    } else if(event.key == InputKeyBack) {
                        // Exit search results mode
                        app->showing_search_results = false;
                        
                        // Free search results
                        if(app->search_results != NULL) {
                            free(app->search_results);
                            app->search_results = NULL;
                            app->search_results_count = 0;
                        }
                    } else if(event.key == InputKeyUp) {
                        // Navigate to previous search result
                        if(app->current_search_index > 0 && app->search_results_count > 0) {
                            app->current_search_index--;
                        }
                    } else if(event.key == InputKeyDown) {
                        // Navigate to next search result
                        if(app->current_search_index < app->search_results_count - 1) {
                            app->current_search_index++;
                        }
                    } else if(event.key == InputKeyRight) {
                        // Add current search result to favorites
                        if(app->search_results_count > 0) {
                            uint32_t word_index = app->search_results[app->current_search_index];
                            add_to_favorites(app, word_index);
                        }
                    }
                }
            } else if(app->showing_favorites) {
                // Favorites list navigation
                if(event.type == InputTypePress) {
                    if(event.key == InputKeyOk) {
                        if(app->favorites_count > 0) {
                            // Show definition of the selected favorite
                            app->current_word_index = app->favorites[app->current_favorite_index];
                            
                            // Get the word from index and copy it to search_term for showing definition
                            const char* word = dictionary_data_get_word(app->current_word_index);
                            strncpy(app->search_term, word, sizeof(app->search_term) - 1);
                            app->search_term[sizeof(app->search_term) - 1] = '\0';
                            app->search_term_length = strlen(app->search_term);
                            
                            app->showing_definition = true;
                            app->scroll_position = 0;
                        }
                    } else if(event.key == InputKeyBack) {
                        // Exit favorites mode
                        app->showing_favorites = false;
                    } else if(event.key == InputKeyUp) {
                        // Navigate to previous favorite
                        if(app->current_favorite_index > 0 && app->favorites_count > 0) {
                            app->current_favorite_index--;
                        }
                    } else if(event.key == InputKeyDown) {
                        // Navigate to next favorite
                        if(app->current_favorite_index < app->favorites_count - 1) {
                            app->current_favorite_index++;
                        }
                    } else if(event.key == InputKeyRight) {
                        // Remove from favorites
                        if(app->favorites_count > 0) {
                            uint32_t word_index = app->favorites[app->current_favorite_index];
                            remove_from_favorites(app, word_index);
                        }
                    }
                }
            } else {
                // Main dictionary navigation
                if(event.type == InputTypePress) {
                    if(event.key == InputKeyOk) {
                        // Start searching
                        app->is_searching = true;
                        
                        // Initialize search term with empty string
                        app->search_term[0] = '\0';
                        app->search_term_length = 0;
                        
                        // Cancel any previous search results
                        app->showing_search_results = false;
                        if(app->search_results != NULL) {
                            free(app->search_results);
                            app->search_results = NULL;
                            app->search_results_count = 0;
                        }
                    } else if(event.key == InputKeyBack) {
                        // Exit application
                        running = false;
                    } else if(event.key == InputKeyUp) {
                        // Navigate to previous word
                        if(app->current_word_index > 0) {
                            app->current_word_index--;
                        }
                    } else if(event.key == InputKeyDown) {
                        // Navigate to next word
                        if(app->current_word_index < dictionary_data_get_word_count() - 1) {
                            app->current_word_index++;
                        }
                    } else if(event.key == InputKeyRight) {
                        // Toggle favorite status for current word
                        if(is_word_favorite(app, app->current_word_index)) {
                            remove_from_favorites(app, app->current_word_index);
                        } else {
                            add_to_favorites(app, app->current_word_index);
                        }
                    } else if(event.key == InputKeyLeft) {
                        // Show favorites
                        app->showing_favorites = true;
                        app->current_favorite_index = 0;
                    }
                }
            }
            
            // Update the display
            view_port_update(app->view_port);
        }
    }

    // Clean up
    dictionary_app_free(app);
    free(app);
    
    return 0;
}
