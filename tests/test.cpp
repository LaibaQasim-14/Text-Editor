
#include <iostream>
#include <cassert>
#include <sstream>
#include "DoublyLinkedList.h"

using namespace std;

int main()
{
    DoublyLinkedList list;

    // Test 1: Empty list
    assert(list.isEmpty());
    assert(list.getSize() == 0);
    cout << "Test 1 passed: Empty list\n";

    // Test 2: Insert first node
    assert(list.insertAt(0, "A"));
    assert(list.getSize() == 1);
    assert(list.getAt(0)->text == "A");
    cout << "Test 2 passed: Insert first node\n";

    // Test 3: Insert at the beginning
    assert(list.insertAt(0, "B"));
    assert(list.getAt(0)->text == "B");
    assert(list.getAt(1)->text == "A");
    cout << "Test 3 passed: Insert at beginning\n";

    // Test 4: Insert at the end
    assert(list.insertAt(2, "C"));
    assert(list.getAt(2)->text == "C");
    cout << "Test 4 passed: Insert at end\n";

    // Test 5: Insert in the middle
    assert(list.insertAt(1, "D"));
    assert(list.getAt(0)->text == "B");
    assert(list.getAt(1)->text == "D");
    assert(list.getAt(2)->text == "A");
    assert(list.getAt(3)->text == "C");
    cout << "Test 5 passed: Insert in middle\n";

    // Test 6: Invalid indexes
    assert(!list.insertAt(-1, "X"));
    assert(!list.insertAt(10, "X"));
    assert(list.getAt(-1) == nullptr);
    assert(list.getAt(10) == nullptr);
    cout << "Test 6 passed: Invalid indexes\n";

    // Test 7: Delete from the beginning
    assert(list.deleteAt(0));
    assert(list.getAt(0)->text == "D");
    cout << "Test 7 passed: Delete from beginning\n";

    // Test 8: Delete from the middle
    assert(list.deleteAt(1));
    assert(list.getAt(0)->text == "D");
    assert(list.getAt(1)->text == "C");
    cout << "Test 8 passed: Delete from middle\n";

    // Test 9: Delete from the end
    assert(list.deleteAt(1));
    assert(list.getSize() == 1);
    assert(list.getAt(0)->text == "D");
    cout << "Test 9 passed: Delete from end\n";

    // Test 10: Invalid deletion
    assert(!list.deleteAt(-1));
    assert(!list.deleteAt(5));
    assert(list.getSize() == 1);
    cout << "Test 10 passed: Invalid deletion\n";

    // Test 11: Clear the list
    list.clear();
    assert(list.isEmpty());
    assert(list.getSize() == 0);
    assert(list.getHead() == nullptr);
    assert(list.getTail() == nullptr);
    cout << "Test 11 passed: Clear list\n";

    // Test 12: Reuse after clearing
    assert(list.insertAt(0, "Again"));
    assert(list.getAt(0)->text == "Again");
    list.clear();
    cout << "Test 12 passed: Reuse after clear\n";

    // Test 13: Display
    list.insertAt(0, "Hello");
    list.insertAt(1, "World");

    ostringstream output;
    list.display(output);

    assert(output.str().find("Hello") != string::npos);
    assert(output.str().find("World") != string::npos);
    cout << "Test 13 passed: Display\n";

    cout << "\nAll tests passed!\n";

    return 0;
}

