# Flipper Zero Dictionary Application

A multilingual dictionary application for Flipper Zero that allows users to browse words, look up definitions, save favorites, search for specific entries, and toggle between English definitions and Russian translations.

## Features

- **Dictionary Browsing**: Navigate through words alphabetically
- **Definitions and Translations**: View English definitions or Russian translations for each word
- **Favorites System**: Mark/unmark favorite words and browse your favorites list
- **Search Functionality**: Search for specific words in the dictionary
- **User-Friendly Interface**: Intuitive navigation using Flipper Zero buttons

## Controls

### Main Dictionary View
- **UP/DOWN**: Navigate through words
- **OK**: View definition/translation of selected word
- **LEFT**: Access favorites list
- **RIGHT**: Toggle favorite status for the current word
- **BACK**: Exit the application

### Word View (Definition/Translation)
- **LEFT**: Toggle between English definition and Russian translation
- **RIGHT**: Toggle favorite status
- **BACK**: Return to word list

### Favorites View
- **UP/DOWN**: Navigate through favorite words
- **OK**: View definition/translation of selected favorite
- **RIGHT**: Remove word from favorites
- **BACK**: Return to main dictionary view

### Search View
- **UP/DOWN**: Change current letter
- **LEFT/RIGHT**: Move cursor position
- **OK**: Execute search with current term
- **BACK**: Cancel search and return to dictionary

## Installation

1. Clone this repository or download the source files
2. Build using the standard Flipper Zero build process:
   ```
   ./fbt fap_dictionary
   ```
3. Install the resulting `.fap` file to your Flipper Zero

## Development Notes

- The dictionary data structure supports both English definitions and Russian translations
- Favorites are saved to persistent storage
- The application uses standard Flipper Zero UI elements and input handling

## License

This application is released under the MIT License. See the LICENSE file for details.
