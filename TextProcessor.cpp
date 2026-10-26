#include "TextProcessor.h"

int TextProcessor::findInString(
    const std::string& text,
    const std::string& searchText,
    int startPosition
) {
    // Empty patterns are rejected to avoid confusing matches and loops.
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

    // Naive string-search algorithm.
    for (int i = startPosition; i <= textLength - patternLength; ++i) {
        int j = 0;

        while (j < patternLength && text[i + j] == searchText[j]) {
            ++j;
        }

        if (j == patternLength) {
            return i;
        }
    }

    return -1;
}

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

    for (int lineIndex = 0;
         lineIndex < document.getLineCount();
         ++lineIndex) {
        const Line* line = document.getLine(lineIndex);

        if (line == nullptr) {
            continue;
        }

        int startPosition = 0;
                    if (foundPosition == -1) {
                break;
            }

            results.push_back(Match(lineIndex, foundPosition));

            if (allowOverlapping) {
                // Move by one character so overlapping matches are included.
                startPosition = foundPosition + 1;
            } else {
                // Move past the match so results do not overlap.
                startPosition =
                    foundPosition + static_cast<int>(searchText.length());
            }
        }
    }

    return true;
}

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

    const int foundPosition =
        findInString(line->text, searchText, position.col);

    // Require an exact match at the requested column; don't silently replace
    // a later occurrence if the supplied position is stale or incorrect.
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

    record = ReplacementRecord(
        position.line,
        position.col,
        searchText,
        replacementText
    );

    return true;
}

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

    // Process each line separately. Positions in records refer to the
    // document state at the moment each replacement was made. Undo should
    // apply records in reverse order.
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
            const int foundPosition =
                findInString(updatedText, searchText, startPosition);

            if (foundPosition == -1) {
                break;
            }

            // Save the old/new text and the position in the current,
            // progressively updated line.
            records.push_back(ReplacementRecord(
                lineIndex,
                foundPosition,
                searchText,
                replacementText
            ));

            updatedText.replace(
                static_cast<std::string::size_type>(foundPosition),
                searchText.length(),
                replacementText
            );

            // Critical: advance past the newly inserted replacement text.
            // For example, replacing "a" with "aa" will not loop forever.
            startPosition =
                foundPosition + static_cast<int>(replacementText.length());
        }

        if (updatedText != line->text) {
            if (!document.setLine(lineIndex, updatedText)) {
                return false;
                }
        }
    }

    return true;
}
