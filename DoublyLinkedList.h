
#ifndef DOUBLYLINKEDLIST_H
#define DOUBLYLINKEDLIST_H

#include "Line.h"
#include <string>
#include <iosfwd>

class DoublyLinkedList
{
private:
    Line* head;
    Line* tail;
    int size;

public:
    DoublyLinkedList();
    ~DoublyLinkedList();

    // Prevent accidental shallow copying of owned nodes
    DoublyLinkedList(const DoublyLinkedList&) = delete;
    DoublyLinkedList& operator=(const DoublyLinkedList&) = delete;

    bool insertAt(int index, const std::string& text);
    bool deleteAt(int index);

    Line* getAt(int index) const;
    Line* getHead() const;
    Line* getTail() const;

    int getSize() const;
    bool isEmpty() const;

    void clear();
    void display(std::ostream& out) const;
};

#endif
