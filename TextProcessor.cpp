
#include "TextProcessor.h"

// ==========================================
// 1. FIND IN STRING
// ==========================================

int TextProcessor::findInString(
    const std::string& text,
    const std::string& searchText,
    int startPosition
) {
    if (searchText.empty() ||
        startPosition < 0 ||
        startPosition > static_cast<int>(text.length())) {
        return -1;
    }

    const int textLength = static_cast<int>(text.length());
    const int patternLength = static_cast<int>(searchText.length());

    if (patternLength > textLength - startPosition) {
        return -1;
    }

    // Naive string searching algorithm.
    for (int i = startPosition;
         i <= textLength - patternLength; ++i) {

        int j = 0;

        while (j < patternLength &&
               text[i + j] == searchText[j]) {
            ++j;
        }

        if (j == patternLength) {
            return i;
        }
    }

    return -1;
}


// ==========================================
// 2. SEARCH ALL
// ==========================================

bool TextProcessor::searchAll(
    const Document& document,
    const std::string& searchText,
    DynamicArray<Match>& results,
    bool allowOverlapping
) {
    results.clear();

    if (searchText.empty()) {
        return false;
    }

    // Traverse every line in the document.
    for (int lineIndex = 0;
         lineIndex < document.getLineCount();
         ++lineIndex) {

        const Line* line = document.getLine(lineIndex);

        if (line == nullptr) {
            continue;
        }

        int startPosition = 0;

        // Search repeatedly inside the current line.
        while (startPosition <= static_cast<int>(line->text.length())) {

            int foundPosition = findInString(
                line->text,
                searchText,
                startPosition
            );

            if (foundPosition == -1) {
                break;
            }

            // Store the line number and column number.
            results.push_back(
                Match(lineIndex, foundPosition)
            );

            if (allowOverlapping) {
                // Move one character forward.
                startPosition = foundPosition + 1;
            }
            else {
                // Move past the complete matched text.
                startPosition =
                    foundPosition + static_cast<int>(searchText.length());
            }
        }
    }

    return true;
}


// ==========================================
// 3. REPLACE OCCURRENCE
// ==========================================

bool TextProcessor::replaceOccurrence(
    Document& document,
    const Match& position,
    const std::string& searchText,
    const std::string& replacementText,
    ReplacementRecord& record
) {
    if (searchText.empty()) {
        return false;
    }

    const Line* line = document.getLine(position.line);

    if (line == nullptr ||
        position.col < 0 ||
        position.col > static_cast<int>(line->text.length())) {
        return false;
    }

    int foundPosition = findInString(
        line->text,
        searchText,
        position.col
    );

    // Ensure the match is exactly at the requested position.
    if (foundPosition != position.col) {
        return false;
    }

    std::string updatedText = line->text;

    updatedText.replace(
        static_cast<std::string::size_type>(position.col),
        searchText.length(),
        replacementText
    );

    if (!document.setLine(position.line, updatedText)) {
        return false;
    }

    // Save replacement information for Undo.
    record = ReplacementRecord(
        position.line,
        position.col,
        searchText,
        replacementText
    );

    return true;
}


// ==========================================
// 4. REPLACE ALL
// ==========================================

bool TextProcessor::replaceAll(
    Document& document,
    const std::string& searchText,
    const std::string& replacementText,
    DynamicArray<ReplacementRecord>& records
) {
    records.clear();

    if (searchText.empty()) {
        return false;
    }

    // Process each document line separately.
    for (int lineIndex = 0;
         lineIndex < document.getLineCount();
         ++lineIndex) {

        const Line* line = document.getLine(lineIndex);

        if (line == nullptr) {
            continue;
        }

        std::string updatedText = line->text;
        int startPosition = 0;

        while (startPosition <= static_cast<int>(updatedText.length())) {

            int foundPosition = findInString(
                updatedText,
                searchText,
                startPosition
            );

            if (foundPosition == -1) {
                break;
            }

            // Store replacement information for Undo.
            records.push_back(
                ReplacementRecord(
                    lineIndex,
                    foundPosition,
                    searchText,
                    replacementText
                )
            );

            // Perform replacement.
            updatedText.replace(
                static_cast<std::string::size_type>(foundPosition),
                searchText.length(),
                replacementText
            );

            // Move past the newly inserted replacement.
            startPosition =
                foundPosition + static_cast<int>(replacementText.length());
        }

        // Update document only if text has changed.
        if (updatedText != line->text) {

            if (!document.setLine(lineIndex, updatedText)) {
                return false;
            }
        }
    }

    return true;
}
