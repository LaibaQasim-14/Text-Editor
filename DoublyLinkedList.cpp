
#include "DoublyLinkedList.h"
#include <ostream>

DoublyLinkedList::DoublyLinkedList()
{
    head = nullptr;
    tail = nullptr;
    size = 0;
}

DoublyLinkedList::~DoublyLinkedList()
{
    clear();
}

// Insert at any valid position: 0 to size
bool DoublyLinkedList::insertAt(int index, const std::string& text)
{
    if (index < 0 || index > size)
        return false;

    Line* newNode = new Line(text);

    // Case 1: The list is empty
    if (head == nullptr)
    {
        head = newNode;
        tail = newNode;
    }
    // Case 2: Insert at the beginning
    else if (index == 0)
    {
        newNode->next = head;
        head->prev = newNode;
        head = newNode;
    }
    // Case 3: Insert at the end
    else if (index == size)
    {
        newNode->prev = tail;
        tail->next = newNode;
        tail = newNode;
    }
    // Case 4: Insert in the middle
    else
    {
        Line* current = getAt(index);

        newNode->next = current;
        newNode->prev = current->prev;

        current->prev->next = newNode;
        current->prev = newNode;
    }

    size++;
    return true;
}

// Delete a node at any valid position: 0 to size - 1
bool DoublyLinkedList::deleteAt(int index)
{
    if (index < 0 || index >= size)
        return false;

    Line* current = getAt(index);

    // Update the previous node or head
    if (current->prev != nullptr)
        current->prev->next = current->next;
    else
        head = current->next;

    // Update the next node or tail
    if (current->next != nullptr)
        current->next->prev = current->prev;
    else
        tail = current->prev;

    delete current;
    size--;

    return true;
}

// Find a node by index.
// Start from the closer end for better performance.
Line* DoublyLinkedList::getAt(int index) const
{
    if (index < 0 || index >= size)
        return nullptr;

    Line* current;

    if (index < size / 2)
    {
        current = head;

        for (int i = 0; i < index; i++)
            current = current->next;
    }
    else
    {
        current = tail;

        for (int i = size - 1; i > index; i--)
            current = current->prev;
    }

    return current;
}

Line* DoublyLinkedList::getHead() const
{
    return head;
}

Line* DoublyLinkedList::getTail() const
{
    return tail;
}

int DoublyLinkedList::getSize() const
{
    return size;
}

bool DoublyLinkedList::isEmpty() const
{
    return size == 0;
}

// Delete every node and reset the list
void DoublyLinkedList::clear()
{
    Line* current = head;

    while (current != nullptr)
    {
        Line* nextNode = current->next;
        delete current;
        current = nextNode;
    }

    head = nullptr;
    tail = nullptr;
    size = 0;
}

// Write to the caller's stream; do not print errors here
void DoublyLinkedList::display(std::ostream& out) const
{
    Line* current = head;
    int index = 0;

    while (current != nullptr)
    {
        out << index << ": " << current->text << '\n';
        current = current->next;
        index++;
    }
}
