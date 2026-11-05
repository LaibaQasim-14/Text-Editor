
#ifndef DYNAMICARRAY_H
#define DYNAMICARRAY_H

#include <cstddef>
#include <stdexcept>
#include <utility>

template <typename T>
class DynamicArray {
private:
    T* arr;
    int size;
    int capacity;

public:
    // Constructor
    explicit DynamicArray(int initialCapacity = 2)
        : arr(nullptr), size(0),
          capacity(initialCapacity > 0 ? initialCapacity : 2) {
        arr = new T[capacity];
    }

    // Destructor
    ~DynamicArray() {
        delete[] arr;
    }

    // Prevent shallow copying of dynamically allocated memory
    DynamicArray(const DynamicArray&) = delete;
    DynamicArray& operator=(const DynamicArray&) = delete;

    // Insert an element at a valid index (0 to size)
    bool insertAt(int index, const T& value) {
        if (index < 0 || index > size) {
            return false;
        }

        if (size == capacity) {
            resize(capacity * 2);
        }

        for (int i = size; i > index; i--) {
            arr[i] = arr[i - 1];
        }

        arr[index] = value;
        size++;

        return true;
    }

    // Add an element at the end
    void push_back(const T& value) {
        if (size == capacity) {
            resize(capacity * 2);
        }

        arr[size] = value;
        size++;
    }

    // Return a pointer to the element at a valid index
    T* getAt(int index) {
        if (index < 0 || index >= size) {
            return nullptr;
        }

        return &arr[index];
    }

    // Read-only access
    const T* getAt(int index) const {
        if (index < 0 || index >= size) {
            return nullptr;
        }

        return &arr[index];
    }

    // Update an element at a valid index
    bool set(int index, const T& value) {
        if (index < 0 || index >= size) {
            return false;
        }

        arr[index] = value;
        return true;
    }

    // Delete an element at a valid index
    bool deleteAt(int index) {
        if (index < 0 || index >= size) {
            return false;
        }

        for (int i = index; i < size - 1; i++) {
            arr[i] = arr[i + 1];
        }

        size--;

        return true;
    }

    // Return the number of stored elements
    int getSize() const {
        return size;
    }

    // Return the allocated capacity
    int getCapacity() const {
        return capacity;
    }

    // Check whether the array is empty
    bool isEmpty() const {
        return size == 0;
    }

    // Resize the array
    void resize(int newCapacity) {
        if (newCapacity < size || newCapacity <= 0) {
            throw std::invalid_argument(
                "New capacity cannot be smaller than size or zero."
            );
        }

        T* newArr = new T[newCapacity];

        try {
            for (int i = 0; i < size; i++) {
                newArr[i] = arr[i];
            }
        } catch (...) {
            delete[] newArr;
            throw;
        }

        delete[] arr;
        arr = newArr;
        capacity = newCapacity;
    }

    // Remove all elements while keeping allocated capacity
    void clear() {
        size = 0;
    }
};

#endif
