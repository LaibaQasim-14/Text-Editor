#ifndef STACK_H
#define STACK_H

#include <cstddef>

// Array-based stack with automatic resizing.
// push/pop/peek are O(1) (push is amortized O(1)).
// removeBottom is O(k) because it shifts every element down by one;
// it is only used to enforce the undo limit (k = 100).
template <typename T>
class Stack {
private:
    T* arr;
    std::size_t capacity;
    std::size_t top;

    void resize() {
        std::size_t newCapacity = capacity * 2;

        T* newArr = new T[newCapacity];

        for (std::size_t i = 0; i < top; i++) {
            newArr[i] = arr[i];
        }

        delete[] arr;

        arr = newArr;
        capacity = newCapacity;
    }

public:
    Stack() : capacity(2), top(0) {
        arr = new T[capacity];
    }

    ~Stack() {
        delete[] arr;
    }

    Stack(const Stack&) = delete;
    Stack& operator=(const Stack&) = delete;

    void push(const T& value) {
        if (top == capacity) {
            // Copy first: 'value' could refer to an element of arr,
            // which resize() is about to delete.
            T copy = value;
            resize();
            arr[top] = copy;
        } else {
            arr[top] = value;
        }

        top++;
    }

    bool pop(T& value) {
        if (isEmpty()) {
            return false;
        }

        top--;
        value = arr[top];

        return true;
    }

    bool peek(T& value) const {
        if (isEmpty()) {
            return false;
        }

        value = arr[top - 1];

        return true;
    }

    bool isEmpty() const {
        return top == 0;
    }

    std::size_t getSize() const {
        return top;
    }

    void clear() {
        top = 0;
    }

    // Removes the oldest (bottom) element. O(k).
    bool removeBottom() {
        if (isEmpty()) {
            return false;
        }

        for (std::size_t i = 1; i < top; i++) {
            arr[i - 1] = arr[i];
        }

        top--;

        return true;
    }
};

#endif