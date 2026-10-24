#ifndef TEXTUTILS_H
#define TEXTUTILS_H

#include <string>
#include <cstddef>

// All functions work on plain strings and never print.
// They return false (and leave the string unchanged) on invalid input.

// Inserts text at position (0..line.length()).
bool insertInString(
    std::string& line,
    std::size_t position,
    const std::string& text
);

// Deletes 'length' characters starting at position.
// length must be at least 1. deletedText receives the removed characters.
bool deleteInString(
    std::string& line,
    std::size_t position,
    std::size_t length,
    std::string& deletedText
);

// Deletes the word that contains 'position'.
// Fails if position is out of range or points at a space.
// deletedText receives the word; startCol receives the column where
// the word began (needed so Undo can put it back).
bool deleteWordInString(
    std::string& line,
    std::size_t position,
    std::string& deletedText,
    std::size_t& startCol
);

#endif