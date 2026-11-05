
#ifndef TEXTPROCESSOR_H
#define TEXTPROCESSOR_H

#include <string>

#include "Document.h"
#include "DynamicArray.h"
#include "Match.h"

// Stores information required to undo one replacement.
struct ReplacementRecord {
    Match position;
    std::string oldText;
    std::string newText;

    ReplacementRecord(
        int lineNumber = 0,
        int column = 0,
        const std::string& oldValue = "",
        const std::string& newValue = ""
    )
        : position(lineNumber, column),
          oldText(oldValue),
          newText(newValue) {
    }
};

class TextProcessor {
public:

    // Finds the first occurrence of searchText.
    static int findInString(
        const std::string& text,
        const std::string& searchText,
        int startPosition = 0
    );

    // Searches all document lines.
    static bool searchAll(
        const Document& document,
        const std::string& searchText,
        DynamicArray<Match>& results,
        bool allowOverlapping = false
    );

    // Replaces one occurrence at a given position.
    static bool replaceOccurrence(
        Document& document,
        const Match& position,
        const std::string& searchText,
        const std::string& replacementText,
        ReplacementRecord& record
    );

    // Replaces all non-overlapping occurrences.
    static bool replaceAll(
        Document& document,
        const std::string& searchText,
        const std::string& replacementText,
        DynamicArray<ReplacementRecord>& records
    );
};

#endif
