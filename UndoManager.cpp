#include "UndoManager.h"

#include <string>

UndoManager::UndoManager() : limit(100) {
}

void UndoManager::record(const EditOperation& op) {
    // skip empty text edits
    if (op.type == OperationType::INSERT_TEXT && op.newText.empty()) {
        return;
    }

    if (op.type == OperationType::DELETE_TEXT && op.oldText.empty()) {
        return;
    }

    while (history.getSize() >= limit) {
        history.removeBottom();
    }

    history.push(op);
}

bool UndoManager::undo(Document& doc) {
    EditOperation op;

    if (!history.pop(op)) {
        return false;
    }

    std::size_t groupId = op.groupId;

    if (!applyInverse(op, doc)) {
        return false;
    }

    if (groupId == 0) {
        return true;
    }

    // undo the rest of the group
    EditOperation next;

    while (history.peek(next) && next.groupId == groupId) {
        history.pop(next);

        if (!applyInverse(next, doc)) {
            return false;
        }
    }

    return true;
}

bool UndoManager::canUndo() const {
    return !history.isEmpty();
}

void UndoManager::clear() {
    history.clear();
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

bool UndoManager::applyInverse(const EditOperation& op, Document& doc) {
    int line = static_cast<int>(op.line);
    int col = static_cast<int>(op.col);

    switch (op.type) {

    case OperationType::INSERT_TEXT:
        return doc.deleteRange(line, col,
                               static_cast<int>(op.newText.length()));

    case OperationType::DELETE_TEXT:
        return doc.insertText(line, col, op.oldText);

    case OperationType::REPLACE:
        if (!op.newText.empty()) {
            if (!doc.deleteRange(line, col,
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
        // merge line and line + 1
        const Line* first = doc.getLine(line);
        const Line* second = doc.getLine(line + 1);

        if (first == nullptr || second == nullptr) {
            return false;
        }

        std::string merged = first->text + second->text;

        if (!doc.setLine(line, merged)) {
            return false;
        }

        return doc.deleteLine(line + 1);
    }
    }

    return false;
}