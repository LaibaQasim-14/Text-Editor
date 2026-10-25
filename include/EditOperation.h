#ifndef EDITOPERATION_H
#define EDITOPERATION_H

#include <string>
#include <cstddef>

enum class OperationType {
    INSERT_TEXT,
    DELETE_TEXT,
    REPLACE,
    INSERT_LINE,
    DELETE_LINE,
    SPLIT_LINE
};

struct EditOperation {
    OperationType type = OperationType::INSERT_TEXT;

    std::size_t line = 0;   // 0-based
    std::size_t col = 0;    // 0-based

    std::string oldText;
    std::string newText;

    // 0 = ordinary operation (not grouped).
    // Replace All gives every change the same non-zero id (1, 2, 3, ...),
    // so one undo can reverse the whole group.
    std::size_t groupId = 0;
};

#endif