#include "TextUtils.h"

// Inserts text at the specified position.
bool insertInString(
    std::string& line,
    std::size_t position,
    const std::string& text
) {
    if (position > line.length()) {
        return false;
    }

    line.insert(position, text);
    return true;
}

// Deletes a specified number of characters.
bool deleteInString(
    std::string& line,
    std::size_t position,
    std::size_t length,
    std::string& deletedText
) {
    if (position > line.length()) {
        return false;
    }

    if (length > line.length() - position) {
        return false;
    }

    deletedText = line.substr(position, length);
    line.erase(position, length);

    return true;
}

// Deletes the word containing the specified position.
bool deleteWordInString(
    std::string& line,
    std::size_t position,
    std::string& deletedText
) {
    if (position >= line.length()) {
        return false;
    }

    // Find the beginning of the word.
    std::size_t start = position;

    while (start > 0 && line[start - 1] != ' ') {
        --start;
    }

    // Find the end of the word.
    std::size_t end = position;

    while (end < line.length() && line[end] != ' ') {
        ++end;
    }

    // Save the word before deleting it.
    deletedText = line.substr(start, end - start);

    // Remove the word.
    line.erase(start, end - start);

    return true;
}