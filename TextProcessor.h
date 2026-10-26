#ifndef TEXTPROCESSOR_H
#define TEXTPROCESSOR_H

#include <string>

#include "Document.h"
#include "DynamicArray.h"
#include "Match.h"

// Information needed to reverse one replacement.
// For replaceAll(), undo records should be applied in reverse order.
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
    // Returns the first matching index at or after startPosition.
    // Returns -1 when no match is found or the inputs are invalid.
    static int findInString(
        const std::string& text,
        const std::string& searchText,
        int startPosition = 0
    );

    // Searches every line and fills results with zero-based line/column
    // positions. Returns false if searchText is empty.
    // allowOverlapping=true also returns overlapping matches.
    static bool searchAll(
        const Document& document,
        const std::string& searchText,
        DynamicArray<Match>& results,
        bool allowOverlapping = false
    );

    // Replaces one occurrence at the given position.
    // Returns false if the search text is empty, the position is invalid,
    // or the text at that position does not match searchText.
    // On success, record contains the old/new text and position for Undo.
    static bool replaceOccurrence(
        Document& document,
        const Match& position,
        const std::string& searchText,
        const std::string& replacementText,
        ReplacementRecord& record
    );

    // Replaces all non-overlapping occurrences across all document lines.
    // Returns false if searchText is empty.
    // Each record stores the position at the time that replacement occurred.
    // To undo these records safely, process them in reverse order.
    // Returns true when the request is valid, even if no matches were found.
    static bool replaceAll(
        Document& document,
        const std::string& searchText,
        const std::string& replacementText,
        DynamicArray<ReplacementRecord>& records
    );
};

#endif
