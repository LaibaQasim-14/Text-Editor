
#include "UndoStack.h"

// Initially, the stack can store two edit operations.
UndoStack::UndoStack()
    : arr(new EditOperation[2]), capacity(2), top(0) {
}

// Releases the dynamically allocated array.
UndoStack::~UndoStack() {
    delete[] arr;
}

// Creates a larger array when the current array is full.
void UndoStack::resize() {
    std::size_t newCapacity = capacity * 2;

    EditOperation* newArr = new EditOperation[newCapacity];

    // Copy existing operations into the larger array.
    for (std::size_t i = 0; i < top; i++) {
        newArr[i] = arr[i];
    }

    delete[] arr;

    arr = newArr;
    capacity = newCapacity;
}

// Adds an operation to the top of the stack.
void UndoStack::push(const EditOperation& operation) {
    // Increase capacity if no slots are available.
    if (top == capacity) {
        resize();
    }

    arr[top] = operation;
    ++top;
}

// Removes the most recently added operation.
// Returns false if the stack is empty.
bool UndoStack::pop(EditOperation& operation) {
    if (isEmpty()) {
        return false;
    }

    --top;
    operation = arr[top];

    return true;
}

// Reads the top operation without removing it.
bool UndoStack::peek(EditOperation& operation) const {
    if (isEmpty()) {
        return false;
    }

    operation = arr[top - 1];
    return true;
}

// Checks whether the stack contains any operations.
bool UndoStack::isEmpty() const {
    return top == 0;
}

// Returns the number of stored operations.
std::size_t UndoStack::getSize() const {
    return top;
}

// Removes all operations logically, while keeping allocated memory.
void UndoStack::clear() {
    top = 0;
}
