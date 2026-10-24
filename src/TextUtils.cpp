#include "TextUtils.h"

bool insertInString(
    std::string& line,
    std::size_t position,
    const std::string& text
) {
    if (position > line.length()) {
        return false;
    }

    // Built from substr and + only (no insert()).
    line = line.substr(0, position) + text + line.substr(position);

    return true;
}

bool deleteInString(
    std::string& line,
    std::size_t position,
    std::size_t length,
    std::string& deletedText
) {
    if (length == 0) {
        return false;
    }

    if (position >= line.length()) {
        return false;
    }

    if (length > line.length() - position) {
        return false;
    }

    deletedText = line.substr(position, length);

    // Built from substr and + only (no erase()).
    line = line.substr(0, position) + line.substr(position + length);

    return true;
}

bool deleteWordInString(
    std::string& line,
    std::size_t position,
    std::string& deletedText,
    std::size_t& startCol
) {
    if (position >= line.length()) {
        return false;
    }

    // Pointing at a space: there is no word here.
    if (line[position] == ' ') {
        return false;
    }

    // Find the beginning of the word.
    std::size_t start = position;

    while (start > 0 && line[start - 1] != ' ') {
        start--;
    }

    // Find the end of the word.
    std::size_t end = position;

    while (end < line.length() && line[end] != ' ') {
        end++;
    }

    // Save the word and where it started before deleting it.
    deletedText = line.substr(start, end - start);
    startCol = start;

    line = line.substr(0, start) + line.substr(end);

    return true;
}