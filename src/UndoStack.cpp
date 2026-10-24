#include "UndoStack.h"

// Initially, the stack can store two operations.
UndoStack::UndoStack()
    : arr(new EditOperation[2]), capacity(2), top(0) {
}

// Releases the dynamically allocated array.
UndoStack::~UndoStack() {
    delete[] arr;
}

// Doubles the array capacity when the stack is full.
void UndoStack::resize() {
    std::size_t newCapacity = capacity * 2;

    EditOperation* newArr = new EditOperation[newCapacity];

    for (std::size_t i = 0; i < top; i++) {
        newArr[i] = arr[i];
    }

    delete[] arr;

    arr = newArr;
    capacity = newCapacity;
}

// Adds an operation to the top of the stack.
void UndoStack::push(const EditOperation& operation) {
    if (top == capacity) {
        resize();
    }

    arr[top] = operation;
    ++top;
}

// Removes the most recent operation.
bool UndoStack::pop(EditOperation& operation) {
    if (isEmpty()) {
        return false;
    }

    --top;
    operation = arr[top];

    return true;
}

// Reads the most recent operation without removing it.
bool UndoStack::peek(EditOperation& operation) const {
    if (isEmpty()) {
        return false;
    }

    operation = arr[top - 1];
    return true;
}

// Checks whether the stack is empty.
bool UndoStack::isEmpty() const {
    return top == 0;
}

// Returns the number of stored operations.
std::size_t UndoStack::getSize() const {
    return top;
}

// Removes all operations logically.
void UndoStack::clear() {
    top = 0;
}

// Removes the oldest operation.
bool UndoStack::removeBottom() {
    if (isEmpty()) {
        return false;
    }

    for (std::size_t i = 1; i < top; i++) {
        arr[i - 1] = arr[i];
    }

    --top;
    return true;
}
