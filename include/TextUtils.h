#ifndef TEXTUTILS_H
#define TEXTUTILS_H

#include <string>
#include <cstddef>

// Inserts text at the specified position.
bool insertInString(
    std::string& line,
    std::size_t position,
    const std::string& text
);

// Deletes a specified number of characters.
// Stores the deleted text for undo.
bool deleteInString(
    std::string& line,
    std::size_t position,
    std::size_t length,
    std::string& deletedText
);

// Deletes a word at the specified position.
// Stores the deleted text for undo.
bool deleteWordInString(
    std::string& line,
    std::size_t position,
    std::string& deletedText
);

#endif