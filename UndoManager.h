
#ifndef UNDOMANAGER_H
#define UNDOMANAGER_H

#include <cstddef>

#include "Stack.h"
#include "EditOperation.h"
#include "Document.h"

class UndoManager {
private:
    Stack<EditOperation> history;
    Stack<EditOperation> redoHistory;

    std::size_t limit;

    bool applyInverse(const EditOperation& op, Document& doc);
    bool applyForward(const EditOperation& op, Document& doc);

public:
    UndoManager();

    void record(const EditOperation& op);

    bool undo(Document& doc);
    bool redo(Document& doc);

    bool canUndo() const;
    bool canRedo() const;

    void clear();

    bool setLimit(std::size_t newLimit);

    std::size_t getLimit() const;
    std::size_t getSize() const;
};

#endif
