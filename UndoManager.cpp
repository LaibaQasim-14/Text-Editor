
#include "UndoManager.h"
#include <string>

UndoManager::UndoManager() : limit(100) {
}

void UndoManager::record(const EditOperation& op) {

    if (op.type == OperationType::INSERT_TEXT &&
        op.newText.empty()) {
        return;
    }

    if (op.type == OperationType::DELETE_TEXT &&
        op.oldText.empty()) {
        return;
    }

    // A new edit clears the previous redo history.
    redoHistory.clear();

    while (history.getSize() >= limit) {
        history.removeBottom();
    }

    history.push(op);
}

// UNDO
bool UndoManager::undo(Document& doc) {

    EditOperation op;

    if (!history.pop(op)) {
        return false;
    }

    if (!applyInverse(op, doc)) {
        history.push(op);
        return false;
    }

    redoHistory.push(op);

    std::size_t groupId = op.groupId;

    if (groupId == 0) {
        return true;
    }

    EditOperation next;

    while (history.peek(next) &&
           next.groupId == groupId) {

        history.pop(next);

        if (!applyInverse(next, doc)) {
            return false;
        }

        redoHistory.push(next);
    }

    return true;
}

// REDO
bool UndoManager::redo(Document& doc) {

    EditOperation op;

    if (!redoHistory.pop(op)) {
        return false;
    }

    if (!applyForward(op, doc)) {
        redoHistory.push(op);
        return false;
    }

    while (history.getSize() >= limit) {
        history.removeBottom();
    }

    history.push(op);

    std::size_t groupId = op.groupId;

    if (groupId == 0) {
        return true;
    }

    EditOperation next;

    while (redoHistory.peek(next) &&
           next.groupId == groupId) {

        redoHistory.pop(next);

        if (!applyForward(next, doc)) {
            return false;
        }

        while (history.getSize() >= limit) {
            history.removeBottom();
        }

        history.push(next);
    }

    return true;
}

// CHECK UNDO
bool UndoManager::canUndo() const {
    return !history.isEmpty();
}

// CHECK REDO
bool UndoManager::canRedo() const {
    return !redoHistory.isEmpty();
}

// CLEAR BOTH STACKS
void UndoManager::clear() {
    history.clear();
    redoHistory.clear();
}

bool UndoManager::setLimit(std::size_t newLimit) {

    if (newLimit == 0) {
        return false;
    }

    limit = newLimit;

    while (history.getSize() > limit) {
        history.removeBottom();
    }

    return true;
}

std::size_t UndoManager::getLimit() const {
    return limit;
}

std::size_t UndoManager::getSize() const {
    return history.getSize();
}

// APPLY INVERSE OPERATION
bool UndoManager::applyInverse(
    const EditOperation& op,
    Document& doc) {

    int line = static_cast<int>(op.line);
    int col = static_cast<int>(op.col);

    switch (op.type) {

    case OperationType::INSERT_TEXT:
        return doc.deleteRange(
            line, col,
            static_cast<int>(op.newText.length()));

    case OperationType::DELETE_TEXT:
        return doc.insertText(line, col, op.oldText);

    case OperationType::REPLACE:

        if (!op.newText.empty()) {
            if (!doc.deleteRange(
                line, col,
                static_cast<int>(op.newText.length()))) {
                return false;
            }
        }

        return doc.insertText(line, col, op.oldText);

    case OperationType::INSERT_LINE:
        return doc.deleteLine(line);

    case OperationType::DELETE_LINE:
        return doc.insertLine(line, op.oldText);

    case OperationType::SPLIT_LINE: {

        const Line* first = doc.getLine(line);
        const Line* second = doc.getLine(line + 1);

        if (first == nullptr || second == nullptr) {
            return false;
        }

        std::string merged =
            first->text + second->text;

        if (!doc.setLine(line, merged)) {
            return false;
        }

        return doc.deleteLine(line + 1);
    }

    }

    return false;
}

// APPLY FORWARD OPERATION
bool UndoManager::applyForward(
    const EditOperation& op,
    Document& doc) {

    int line = static_cast<int>(op.line);
    int col = static_cast<int>(op.col);

    switch (op.type) {

    case OperationType::INSERT_TEXT:
        return doc.insertText(line, col, op.newText);

    case OperationType::DELETE_TEXT:
        return doc.deleteRange(
            line, col,
            static_cast<int>(op.oldText.length()));

    case OperationType::REPLACE:

        if (!op.oldText.empty()) {
            if (!doc.deleteRange(
                line, col,
                static_cast<int>(op.oldText.length()))) {
                return false;
            }
        }

        return doc.insertText(line, col, op.newText);

    case OperationType::INSERT_LINE:
        return doc.insertLine(line, op.newText);

    case OperationType::DELETE_LINE:
        return doc.deleteLine(line);

    case OperationType::SPLIT_LINE:
        return doc.splitLine(line, col);

    }

    return false;
}
