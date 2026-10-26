#ifndef UNDOMANAGER_H
#define UNDOMANAGER_H

#include <cstddef>

#include "Stack.h"
#include "EditOperation.h"
#include "Document.h"

class UndoManager {
private:
    Stack<EditOperation> history;
    std::size_t limit;

    bool applyInverse(const EditOperation& op, Document& doc);

public:
    UndoManager();

    void record(const EditOperation& op);
    bool undo(Document& doc);   // undoes a whole group if groupId != 0
    bool canUndo() const;
    void clear();
    bool setLimit(std::size_t newLimit);   // newLimit >= 1
    std::size_t getLimit() const;
    std::size_t getSize() const;
};

#endif