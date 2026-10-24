#ifndef EDITOPERATION_H
#define EDITOPERATION_H

#include <string>
#include <cstddef>

// Defines the types of editing operations.
enum class OperationType {
    INSERT_TEXT,
    DELETE_TEXT,
    REPLACE,
    INSERT_LINE,
    DELETE_LINE,
    SPLIT_LINE
};

// Stores information required to undo an operation.
struct EditOperation {
    OperationType type = OperationType::INSERT_TEXT;

    std::size_t lineNumber = 0;
    std::size_t position = 0;

    std::string oldText;
    std::string newText;

    // Groups changes belonging to one Replace All operation.
    std::size_t groupId = 0;
};

#endif
