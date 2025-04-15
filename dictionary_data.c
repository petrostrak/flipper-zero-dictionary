#include "dictionary_data.h"
#include <stdlib.h>
#include <string.h>
#include "furi.h"

// Dictionary entry structure
typedef struct {
    const char* word;
    const char* definition;
    const char* translation;  // Add translation field for bilingual dictionaries
} DictionaryEntry;

// Dictionary data
// This is a sample English-Russian dictionary.
// In a real implementation, this would need to be stored in the Flipper Zero storage
// or compressed in some way to fit more words.
static const DictionaryEntry dictionary_entries[] = {
    {"aardvark", "A large, nocturnal, burrowing mammal native to Africa.", "Трубкозуб"},
    {"abacus", "A calculating device consisting of beads on wires.", "Счёты"},
    {"abandon", "To leave completely and finally; forsake utterly; desert.", "Покидать"},
    {"ability", "Capacity to do or act physically, mentally, legally, morally.", "Способность"},
    {"abode", "A place in which one lives; residence; dwelling; home.", "Жилище"},
    {"book", "A written or printed work consisting of pages.", "Книга"},
    {"cat", "A small domesticated carnivorous mammal with soft fur.", "Кошка"},
    {"dog", "A domesticated carnivorous mammal that typically has a long snout and tail.", "Собака"},
    {"elephant", "A very large plant-eating mammal with a trunk and tusks.", "Слон"},
    {"flower", "The seed-bearing part of a plant, consisting of reproductive organs.", "Цветок"},
    {"guitar", "A stringed musical instrument with a fretted fingerboard.", "Гитара"},
    {"house", "A building used as a home.", "Дом"},
    {"internet", "A global computer network providing information and communication.", "Интернет"},
    {"jungle", "An area of land overgrown with dense forest and vegetation.", "Джунгли"},
    {"kangaroo", "A large hopping Australian marsupial with a long tail.", "Кенгуру"},
    {"language", "The method of human communication, using words.", "Язык"},
    {"music", "Vocal or instrumental sounds combined in a way that produces harmony.", "Музыка"},
    {"notebook", "A small book with blank or ruled pages for writing notes.", "Блокнот"},
    {"orange", "A round juicy citrus fruit with a tough bright reddish-yellow skin.", "Апельсин"},
    {"piano", "A large musical instrument with a keyboard of black and white keys.", "Пианино"},
    {"quiz", "A test of knowledge, especially as a competition.", "Викторина"},
    {"river", "A large natural stream of water flowing in a channel to the sea or a lake.", "Река"},
    {"sun", "The star around which the earth orbits.", "Солнце"},
    {"table", "A piece of furniture with a flat top and one or more legs.", "Стол"},
    {"umbrella", "A folding canopy supported by metal ribs, used as protection against rain.", "Зонт"},
    {"violin", "A stringed musical instrument of treble pitch, played with a bow.", "Скрипка"},
    {"watch", "A small timepiece worn typically on a strap on one's wrist.", "Часы"},
    {"xylophone", "A musical instrument with wooden bars of different lengths.", "Ксилофон"},
    {"yellow", "Of the color between green and orange in the spectrum.", "Жёлтый"},
    {"zebra", "An African wild horse with black-and-white stripes.", "Зебра"},
};

// Number of entries in the dictionary
static const uint32_t dictionary_entry_count = sizeof(dictionary_entries) / sizeof(DictionaryEntry);

// Initialize dictionary data
void dictionary_data_init(void) {
    // No initialization needed for static data
    // In a real implementation, this would load data from storage
}

// Free dictionary data
void dictionary_data_free(void) {
    // No cleanup needed for static data
    // In a real implementation, this would free allocated memory
}

// Get the number of words in the dictionary
uint32_t dictionary_data_get_word_count(void) {
    return dictionary_entry_count;
}

// Get a word at a specific index
const char* dictionary_data_get_word(uint32_t index) {
    if(index < dictionary_entry_count) {
        return dictionary_entries[index].word;
    }
    return NULL;
}

// Get the definition for a word
const char* dictionary_data_get_definition(const char* word) {
    for(uint32_t i = 0; i < dictionary_entry_count; i++) {
        if(strcmp(dictionary_entries[i].word, word) == 0) {
            return dictionary_entries[i].definition;
        }
    }
    
    // Word not found
    return "Definition not found";
}

// Check if a word exists in the dictionary
bool dictionary_data_word_exists(const char* word) {
    for(uint32_t i = 0; i < dictionary_entry_count; i++) {
        if(strcmp(dictionary_entries[i].word, word) == 0) {
            return true;
        }
    }
    
    return false;
}

// Find words starting with a prefix
uint32_t dictionary_data_find_words_with_prefix(const char* prefix, uint32_t** indices) {
    // Count matching words first
    uint32_t count = 0;
    uint32_t prefix_len = strlen(prefix);
    
    for(uint32_t i = 0; i < dictionary_entry_count; i++) {
        if(strncmp(dictionary_entries[i].word, prefix, prefix_len) == 0) {
            count++;
        }
    }
    
    // Allocate array for indices
    *indices = malloc(count * sizeof(uint32_t));
    if(*indices == NULL) {
        return 0;
    }
    
    // Fill array with indices
    uint32_t index = 0;
    for(uint32_t i = 0; i < dictionary_entry_count; i++) {
        if(strncmp(dictionary_entries[i].word, prefix, prefix_len) == 0) {
            (*indices)[index++] = i;
        }
    }
    
    return count;
}

// Find a word index by its string
int32_t dictionary_data_find_word_index(const char* word) {
    for(uint32_t i = 0; i < dictionary_entry_count; i++) {
        if(strcmp(dictionary_entries[i].word, word) == 0) {
            return i;
        }
    }
    
    return -1;
}

// Get the translation for a word (for bilingual dictionaries)
const char* dictionary_data_get_translation(const char* word) {
    for(uint32_t i = 0; i < dictionary_entry_count; i++) {
        if(strcmp(dictionary_entries[i].word, word) == 0) {
            return dictionary_entries[i].translation;
        }
    }
    
    // Translation not found
    return "Translation not available";
}

// Get the translation for a word by its index
const char* dictionary_data_get_translation_by_index(uint32_t index) {
    if(index < dictionary_entry_count) {
        return dictionary_entries[index].translation;
    }
    return "Translation not available";
}
