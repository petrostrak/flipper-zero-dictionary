#include "dictionary_ui.h"
#include "dictionary_app.h"
#include "dictionary_data.h"
#include "dictionary_i.h"

#include "furi.h"
#include "gui/elements.h"
#include "input/input.h"

// Standard C libraries
#include <stddef.h>
#include <stdio.h>
#include <string.h>

// UI drawing callback
void dictionary_ui_draw_callback(Canvas* canvas, void* context) {
    DictionaryApp* app = context;
    
    canvas_clear(canvas);
    canvas_set_font(canvas, FontPrimary);
    
    if(app->is_searching) {
        // Draw search mode UI
        canvas_draw_str(canvas, 2, 10, "Search:");
        
        // Draw search box
        canvas_draw_frame(canvas, 0, 15, 128, 15);
        
        // Draw search term
        if(app->search_term_length > 0) {
            canvas_draw_str(canvas, 5, 27, app->search_term);
        } else {
            canvas_set_font(canvas, FontSecondary);
            canvas_draw_str(canvas, 5, 27, "<enter search term>");
        }
        
        // Draw instructions
        canvas_set_font(canvas, FontSecondary);
        canvas_draw_str(canvas, 2, 42, "Up/Down: change letter");
        canvas_draw_str(canvas, 2, 52, "Left/Right: move cursor");
        
        // Additional helper messages
        if(app->search_term_length > 0) {
            canvas_draw_str(canvas, 2, 62, "OK: search | BACK: delete");
        } else {
            canvas_draw_str(canvas, 2, 62, "OK: search | BACK: cancel");
        }
    } else if(app->showing_definition) {
        // Draw word definition UI
        canvas_set_font(canvas, FontPrimary);
        
        // Get the current word
        const char* word = dictionary_data_get_word(app->current_word_index);
        canvas_draw_str(canvas, 2, 10, word);
        
        // Draw definition or translation
        canvas_set_font(canvas, FontSecondary);
        const char* text;
        
        if(app->show_translation) {
            // Show Russian translation
            text = dictionary_data_get_translation_by_index(app->current_word_index);
            
            // Display mode indicator
            canvas_draw_str(canvas, 110, 10, "[РУС]");
        } else {
            // Show English definition
            text = dictionary_data_get_definition(word);
            
            // Display mode indicator 
            canvas_draw_str(canvas, 110, 10, "[ENG]");
        }
        
        // Handle text wrapping and scrolling
        uint8_t y_pos = 22;
        uint8_t line_height = 10;
        
        uint8_t max_chars_per_line = 21; // Characters that fit on screen width
        uint8_t current_line = 0;
        
        // text variable is already defined above when selecting between definition and translation
        size_t text_length = strlen(text);
        size_t start_pos = 0;
        
        while(start_pos < text_length) {
            // Calculate how many characters fit on this line
            size_t chars_this_line = 0;
            size_t last_space = 0;
            
            for(size_t i = 0; i < max_chars_per_line && (start_pos + i) < text_length; i++) {
                chars_this_line++;
                if(text[start_pos + i] == ' ') {
                    last_space = i;
                }
            }
            
            // If we found a space and didn't reach the end of text, break at the last space
            if(last_space > 0 && (start_pos + chars_this_line) < text_length) {
                chars_this_line = last_space + 1; // Include the space
            }
            
            // Check if this line should be displayed (based on scroll position)
            if(current_line >= app->scroll_position && current_line < app->scroll_position + 4) {
                // Draw the line
                char line_buffer[22]; // Max chars per line + null terminator
                memcpy(line_buffer, text + start_pos, chars_this_line);
                line_buffer[chars_this_line] = '\0';
                
                canvas_draw_str(
                    canvas, 
                    2, 
                    y_pos + (current_line - app->scroll_position) * line_height, 
                    line_buffer);
            }
            
            // Move to next line
            start_pos += chars_this_line;
            current_line++;
        }
        
        // Draw scroll indicators if needed
        if(app->scroll_position > 0) {
            canvas_draw_str(canvas, 118, 12, "^");
        }
        
        int total_lines = dictionary_ui_count_lines(text);
        if((int)(app->scroll_position + 4) < total_lines) {
            canvas_draw_str(canvas, 118, 62, "v");
        }
        
        // Draw navigation instructions for definition/translation view
        canvas_set_font(canvas, FontSecondary);
        canvas_draw_str(canvas, 2, 62, "←: toggle | ↑↓: scroll");
    } else if(app->showing_search_results) {
        // Draw search results UI
        canvas_draw_str(canvas, 2, 10, "Search Results");
        
        if(app->search_results_count == 0) {
            // No results message
            canvas_set_font(canvas, FontSecondary);
            canvas_draw_str(canvas, 2, 35, "No matching words found");
            canvas_draw_str(canvas, 2, 45, "Press BACK to return");
        } else {
            // Show search term
            canvas_set_font(canvas, FontSecondary);
            char search_label[60]; // Increased buffer size
            snprintf(search_label, sizeof(search_label), "Term: %s (%u results)", 
                    app->search_term, app->search_results_count);
            canvas_draw_str(canvas, 5, 20, search_label);
            
            // Draw search results list
            canvas_set_font(canvas, FontPrimary);
            uint32_t first_visible = 0;
            
            // Adjust view to keep selection visible
            if(app->current_search_index > 1) {
                first_visible = app->current_search_index - 1;
            }
            
            // Draw visible search results
            for(uint32_t i = 0; i < 3 && (first_visible + i) < app->search_results_count; i++) {
                uint32_t word_index = app->search_results[first_visible + i];
                const char* word = dictionary_data_get_word(word_index);
                
                // Check if this word is a favorite
                bool is_favorite = false;
                for(uint8_t f = 0; f < app->favorites_count; f++) {
                    if(app->favorites[f] == word_index) {
                        is_favorite = true;
                        break;
                    }
                }
                
                // Highlight selected search result
                if(first_visible + i == app->current_search_index) {
                    canvas_draw_frame(canvas, 0, 25 + i * 10, 128, 12);
                    if(is_favorite) {
                        // Add star for favorites
                        canvas_draw_str(canvas, 5, 35 + i * 10, "★ ");
                        canvas_draw_str(canvas, 17, 35 + i * 10, word);
                    } else {
                        canvas_draw_str(canvas, 5, 35 + i * 10, word);
                    }
                } else {
                    if(is_favorite) {
                        // Add star for favorites
                        canvas_draw_str(canvas, 5, 35 + i * 10, "★ ");
                        canvas_draw_str(canvas, 17, 35 + i * 10, word);
                    } else {
                        canvas_draw_str(canvas, 5, 35 + i * 10, word);
                    }
                }
            }
            
            // Draw navigation instructions
            canvas_set_font(canvas, FontSecondary);
            canvas_draw_str(canvas, 2, 62, "OK: view | →: ★ | ↓↑: move");
        }
} else if(app->showing_favorites) {
        // Draw favorites UI
        canvas_draw_str(canvas, 2, 10, "Favorites");
        
        if(app->favorites_count == 0) {
            // No favorites message
            canvas_set_font(canvas, FontSecondary);
            canvas_draw_str(canvas, 2, 35, "No favorites added yet");
            canvas_draw_str(canvas, 2, 45, "Press BACK to return");
        } else {
            // Draw favorites list
            uint8_t first_visible = 0;
            
            // Adjust view to keep selection visible
            if(app->current_favorite_index > 2) {
                first_visible = app->current_favorite_index - 2;
            }
            
            // Draw visible favorites
            for(uint8_t i = 0; i < 5 && (first_visible + i) < app->favorites_count; i++) {
                uint32_t word_index = app->favorites[first_visible + i];
                const char* word = dictionary_data_get_word(word_index);
                
                // Highlight selected favorite
                if(first_visible + i == app->current_favorite_index) {
                    canvas_draw_frame(canvas, 0, 15 + i * 10, 128, 12);
                    canvas_draw_str(canvas, 5, 25 + i * 10, word);
                } else {
                    canvas_draw_str(canvas, 5, 25 + i * 10, word);
                }
            }
            
            // Draw navigation instructions
            canvas_set_font(canvas, FontSecondary);
            canvas_draw_str(canvas, 2, 62, "OK: view | →: remove | ↓↑: move");
        }
    } else {
        // Draw main dictionary UI
        canvas_draw_str(canvas, 2, 10, "Dictionary");
        
        // Draw the current word list
        uint32_t word_count = dictionary_data_get_word_count();
        uint32_t first_visible = 0;
        
        // Adjust view to keep selection visible
        if(app->current_word_index > 2) {
            first_visible = app->current_word_index - 2;
        }
        
        // Draw visible words
        for(uint32_t i = 0; i < 5 && first_visible + i < word_count; i++) {
            const char* word = dictionary_data_get_word(first_visible + i);
            uint32_t word_index = first_visible + i;
            
            // Check if this word is a favorite
            bool is_favorite = false;
            for(uint8_t f = 0; f < app->favorites_count; f++) {
                if(app->favorites[f] == word_index) {
                    is_favorite = true;
                    break;
                }
            }
            
            // Highlight selected word
            if(first_visible + i == app->current_word_index) {
                canvas_draw_frame(canvas, 0, 15 + i * 10, 128, 12);
                if(is_favorite) {
                    // Add star for favorites
                    canvas_draw_str(canvas, 5, 25 + i * 10, "★ ");
                    canvas_draw_str(canvas, 17, 25 + i * 10, word);
                } else {
                    canvas_draw_str(canvas, 5, 25 + i * 10, word);
                }
            } else {
                if(is_favorite) {
                    // Add star for favorites
                    canvas_draw_str(canvas, 5, 25 + i * 10, "★ ");
                    canvas_draw_str(canvas, 17, 25 + i * 10, word);
                } else {
                    canvas_draw_str(canvas, 5, 25 + i * 10, word);
                }
            }
        }
        
        // Draw navigation instructions
        canvas_set_font(canvas, FontSecondary);
        canvas_draw_str(canvas, 2, 62, "OK: search | ←: favs | →: ★/☆");
    }
}

// UI input handling callback for the ViewPort
void dictionary_ui_input_callback(InputEvent* input_event, void* ctx) {
    // Forward events to the app's event queue
    furi_assert(ctx != NULL);
    DictionaryApp* app = ctx;
    
    // Put event into the queue
    furi_message_queue_put(app->event_queue, input_event, FuriWaitForever);
}

// Count the number of lines a text would take when rendered
int dictionary_ui_count_lines(const char* text) {
    if(text == NULL) return 0;
    
    int lines = 1;
    size_t max_chars_per_line = 21;
    size_t text_length = strlen(text);
    size_t start_pos = 0;
    
    while(start_pos < text_length) {
        // Calculate how many characters fit on this line
        size_t chars_this_line = 0;
        size_t last_space = 0;
        
        for(size_t i = 0; i < max_chars_per_line && (start_pos + i) < text_length; i++) {
            chars_this_line++;
            if(text[start_pos + i] == ' ') {
                last_space = i;
            }
        }
        
        // If we found a space and didn't reach the end of text, break at the last space
        if(last_space > 0 && (start_pos + chars_this_line) < text_length) {
            chars_this_line = last_space + 1; // Include the space
        }
        
        // Move to next line
        start_pos += chars_this_line;
        if(start_pos < text_length) {
            lines++;
        }
    }
    
    return lines;
}
