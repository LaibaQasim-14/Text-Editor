
#ifndef UNDOSTACK_H //not defined
#define UNDOSTACK_H // Header guard to prevent multiple inclusions of this header file

#include <string> // Include the string header for using std::string
#include <cstddef> // Include the cstddef header for using std::size_t

// Stores information about an edit so it can be undone later.
struct EditOperation {
    enum class Type { //to define the type of edit operation
        Insert,
        Delete,
        Replace,
    };

    Type type = Type::Insert;

    std::size_t lineNumber = 0; // The line number where the edit operation occurred.
    std::size_t position = 0; // The position within the line where the edit operation occurred.

    // Text needed to reverse the operation.
    std::string oldText; //stores the text that was replaced or deleted, allowing for undoing the operation.
    std::string newText;//stores the text that was inserted or replaced, allowing for undoing the operation.
};

class UndoStack {
private:
    EditOperation* arr;      // Dynamic array storing operations
    std::size_t capacity;    // Maximum number of stored operations
    std::size_t top;         // Number of operations currently stored

    // Doubles the array size when the stack becomes full.
    void resize();

public:
    UndoStack();
    ~UndoStack();

    // Prevents copying the raw dynamic array accidentally.
    //copy constructor 
    UndoStack(const UndoStack&) = delete;
    UndoStack& operator=(const UndoStack&) = delete;

    void push(const EditOperation& operation);
    bool pop(EditOperation& operation);
    bool peek(EditOperation& operation) const;

    bool isEmpty() const;
    std::size_t getSize() const;
    void clear();
};

#endif
