#ifndef UNDOSTACK_H
#define UNDOSTACK_H

#include <cstddef>
#include "EditOperation.h"

class UndoStack {
private:
    EditOperation* arr;
    std::size_t capacity;
    std::size_t top;

    void resize();

public:
    UndoStack();
    ~UndoStack();

    // Prevent accidental copying of the dynamic array.
    UndoStack(const UndoStack&) = delete;
    UndoStack& operator=(const UndoStack&) = delete;

    void push(const EditOperation& operation);

    bool pop(EditOperation& operation);
    bool peek(EditOperation& operation) const;

    bool isEmpty() const;
    std::size_t getSize() const;

    void clear();

    // Removes the oldest operation.
    bool removeBottom();
};

#endif
