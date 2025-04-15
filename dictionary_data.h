#pragma once

#include <stdint.h>
#include <stdbool.h>

// Initialize dictionary data
void dictionary_data_init(void);

// Free dictionary data
void dictionary_data_free(void);

// Get the number of words in the dictionary
uint32_t dictionary_data_get_word_count(void);

// Get a word at a specific index
const char* dictionary_data_get_word(uint32_t index);

// Get the definition for a word
const char* dictionary_data_get_definition(const char* word);

// Check if a word exists in the dictionary
bool dictionary_data_word_exists(const char* word);

// Find words starting with a prefix
uint32_t dictionary_data_find_words_with_prefix(const char* prefix, uint32_t** indices);

// Find a word index by its string
int32_t dictionary_data_find_word_index(const char* word);

// Get the translation for a word (for bilingual dictionaries)
const char* dictionary_data_get_translation(const char* word);

// Get the translation for a word by its index
const char* dictionary_data_get_translation_by_index(uint32_t index);
