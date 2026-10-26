
#include <iostream>
#include <string>
#include "DynamicArray.h"
#include "Match.h"

using namespace std;

// Count automated test results
int passedTests = 0;
int failedTests = 0;

// Print a test result
void reportTest(const string& name, bool passed,
                const string& expected,
                const string& actual) {
    cout << "\nTest: " << name << '\n';
    cout << "Expected: " << expected << '\n';
    cout << "Actual:   " << actual << '\n';
    cout << "Result:   " << (passed ? "PASS" : "FAIL") << '\n';

    if (passed)
        passedTests++;
    else
        failedTests++;
}

// Display document lines
void displayArray(DynamicArray<string>& arr) {
    if (arr.isEmpty()) {
        cout << "Document is empty.\n";
        return;
    }

    cout << "\nDocument lines:\n";

    for (int i = 0; i < arr.getSize(); i++) {
        const string* line = arr.getAt(i);

        if (line != nullptr)
            cout << i << ": " << *line << '\n';
    }
}

// Display size and capacity
void showInfo(DynamicArray<string>& arr) {
    cout << "\nSize: " << arr.getSize() << '\n';
    cout << "Capacity: " << arr.getCapacity() << '\n';
    cout << "Empty: "
         << (arr.isEmpty() ? "Yes" : "No") << '\n';
}

// Test set()
void testSet() {
    cout << "\n===== Testing set() =====\n";

    DynamicArray<string> arr;
    arr.push_back("Apple");
    arr.push_back("Mango");
    arr.push_back("Orange");

    bool updated = arr.set(1, "Banana");

    reportTest(
        "set() updates a valid index",
        updated && *arr.getAt(1) == "Banana",
        "Banana at index 1",
        *arr.getAt(1)
    );

    reportTest(
        "set() rejects negative index",
        !arr.set(-1, "Invalid"),
        "Rejected",
        !arr.set(-1, "Invalid") ? "Rejected" : "Accepted"
    );

    int oldSize = arr.getSize();

    bool invalidUpdate = arr.set(arr.getSize(), "Invalid");

    reportTest(
        "set() rejects index equal to size",
        !invalidUpdate && arr.getSize() == oldSize,
        "Rejected; size unchanged",
        !invalidUpdate && arr.getSize() == oldSize
            ? "Rejected; size unchanged"
            : "Unexpected result"
    );

    displayArray(arr);
    showInfo(arr);
}

// Test Match search-result storage
void testMatch() {
    cout << "\n===== Testing DynamicArray<Match> =====\n";

    DynamicArray<Match> results;

    results.push_back(Match(3, 5));
    results.push_back(Match(7, 2));

    const Match* first = results.getAt(0);
    const Match* second = results.getAt(1);

    bool storedCorrectly =
        results.getSize() == 2 &&
        first != nullptr && first->line == 3 && first->col == 5 &&
        second != nullptr && second->line == 7 && second->col == 2;

    reportTest(
        "Store and retrieve Match objects",
        storedCorrectly,
        "Two correct search results",
        storedCorrectly ? "Both results correct" : "Incorrect results"
    );

    bool updated = results.set(0, Match(4, 6));
    first = results.getAt(0);

    bool updateCorrectly =
        updated && first != nullptr &&
        first->line == 4 && first->col == 6 &&
        results.getSize() == 2;

    reportTest(
        "Update a Match without changing size",
        updateCorrectly,
        "Line 4, column 6; size 2",
        updateCorrectly ? "Updated; size remains 2" : "Update failed"
    );
}

// Run the complete Phase 1 checklist
void runChecklist() {
    passedTests = 0;
    failedTests = 0;

    cout << "\n========== PHASE 1 TEST CHECKLIST ==========\n";

    // 1. Empty structure
    {
        DynamicArray<string> arr;

        bool passed =
            arr.getSize() == 0 &&
            arr.isEmpty() &&
            arr.getCapacity() >= 1;

        reportTest(
            "Empty array",
            passed,
            "Size 0; empty Yes; positive capacity",
            passed ? "Correct" : "Incorrect"
        );
    }

    // 2. Wrong indexes
    {
        DynamicArray<string> arr;

        bool invalidInsert =
            !arr.insertAt(-1, "A") &&
            !arr.insertAt(1, "B");

        bool invalidAccess =
            arr.getAt(-1) == nullptr &&
            arr.getAt(0) == nullptr;

        bool invalidDelete =
            !arr.deleteAt(-1) &&
            !arr.deleteAt(0);

        bool invalidSet =
            !arr.set(-1, "A") &&
            !arr.set(0, "B");

        bool passed =
            invalidInsert && invalidAccess &&
            invalidDelete && invalidSet &&
            arr.getSize() == 0;

        reportTest(
            "Reject invalid indexes",
            passed,
            "Invalid operations rejected; size remains 0",
            passed ? "All invalid indexes rejected" : "Check failed"
        );
    }

    // 3. First and last positions
    {
        DynamicArray<string> arr;

        bool inserted =
            arr.insertAt(0, "Middle") &&
            arr.insertAt(0, "First") &&
            arr.insertAt(arr.getSize(), "Last");

        const string* first = arr.getAt(0);
        const string* last = arr.getAt(arr.getSize() - 1);

        bool passed =
            inserted &&
            first != nullptr && *first == "First" &&
            last != nullptr && *last == "Last";

        reportTest(
            "First and last positions",
            passed,
            "First at index 0; Last at final index",
            passed ? "Both positions correct" : "Position error"
        );

        // Check deleting first and last
        bool deletedFirst = arr.deleteAt(0);
        bool deletedLast = arr.deleteAt(arr.getSize() - 1);

        bool deletePassed =
            deletedFirst && deletedLast &&
            arr.getSize() == 1;

        reportTest(
            "Delete first and last positions",
            deletePassed,
            "Both deletions succeed; one element remains",
            deletePassed ? "Correct" : "Incorrect"
        );
    }

    // 4. Automatic resizing
    {
        DynamicArray<string> arr;

        int initialCapacity = arr.getCapacity();

        arr.push_back("One");
        arr.push_back("Two");
        arr.push_back("Three");

        const string* first = arr.getAt(0);
        const string* third = arr.getAt(2);

        bool passed =
            arr.getCapacity() == initialCapacity * 2 &&
            arr.getSize() == 3 &&
            first != nullptr && *first == "One" &&
            third != nullptr && *third == "Three";

        reportTest(
            "Automatic resizing",
            passed,
            "Capacity doubles; all values preserved",
            passed ? "Capacity doubled; values preserved"
                   : "Resize check failed"
        );
    }

    // 5. Clear
    {
        DynamicArray<string> arr;

        arr.push_back("One");
        arr.push_back("Two");

        int oldCapacity = arr.getCapacity();
        arr.clear();

        bool passed =
            arr.getSize() == 0 &&
            arr.isEmpty() &&
            arr.getCapacity() == oldCapacity;

        reportTest(
            "Clear array",
            passed,
            "Size 0; empty Yes; capacity retained",
            passed ? "Correct" : "Incorrect"
        );
    }

    // 6. set()
    {
        DynamicArray<string> arr;

        arr.push_back("Old");
        bool updated = arr.set(0, "New");

        const string* value = arr.getAt(0);

        bool passed =
            updated && value != nullptr &&
            *value == "New" && arr.getSize() == 1;

        reportTest(
            "Update an existing element",
            passed,
            "Value changes; size stays 1",
            passed ? "Correct" : "Incorrect"
        );
    }

    // 7. Match
    {
        DynamicArray<Match> results;

        results.push_back(Match(3, 5));
        results.push_back(Match(7, 2));

        const Match* first = results.getAt(0);
        const Match* second = results.getAt(1);

        bool passed =
            results.getSize() == 2 &&
            first != nullptr && first->line == 3 && first->col == 5 &&
            second != nullptr && second->line == 7 && second->col == 2;

        reportTest(
            "DynamicArray<Match>",
            passed,
            "Both line/column pairs stored correctly",
            passed ? "Both search results correct"
                   : "Match check failed"
        );
    }

    // 8. Summary
    cout << "\n========== TEST SUMMARY ==========\n";
    cout << "Passed: " << passedTests << '\n';
    cout << "Failed: " << failedTests << '\n';

    if (failedTests == 0)
        cout << "All automated checks passed.\n";
    else
        cout << "Some checks failed. Review the results above.\n";

    cout << "\nNote: This test does not prove that no memory "
            "leaks exist.\n";
}

// Main Text Editor test menu
int main() {
    DynamicArray<string> arr;

    int choice;
    int index;
    string value;

    do {
        cout << "\n========== TEXT EDITOR TEST MENU ==========\n";
        cout << "1. Add a line\n";
        cout << "2. Insert a line at an index\n";
        cout << "3. Update a line using set()\n";
        cout << "4. Delete a line\n";
        cout << "5. Display document lines\n";
        cout << "6. Show size and capacity\n";
        cout << "7. Clear document\n";
        cout << "8. Test set()\n";
        cout << "9. Test search results (Match)\n";
        cout << "10. Run complete Phase 1 checklist\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";

        if (!(cin >> choice)) {
            cout << "Error: Please enter a number.\n";
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }

        switch (choice) {
        case 1:
            cout << "Enter line: ";
            getline(cin >> ws, value);
            arr.push_back(value);
            cout << "Line added.\n";
            break;

        case 2:
            cout << "Enter index (0 to " << arr.getSize() << "): ";

            if (!(cin >> index)) {
                cout << "Error: Invalid index input.\n";
                cin.clear();
                cin.ignore(10000, '\n');
                break;
            }

            cout << "Enter line: ";
            getline(cin >> ws, value);

            if (arr.insertAt(index, value))
                cout << "Line inserted.\n";
            else
                cout << "Error: Invalid index.\n";
            break;

        case 3:
            cout << "Enter index to update: ";

            if (!(cin >> index)) {
                cout << "Error: Invalid index input.\n";
                cin.clear();
                cin.ignore(10000, '\n');
                break;
            }

            cout << "Enter new line: ";
            getline(cin >> ws, value);

            if (arr.set(index, value))
                cout << "Line updated.\n";
            else
                cout << "Error: Invalid index.\n";
            break;

        case 4:
            cout << "Enter index to delete: ";

            if (!(cin >> index)) {
                cout << "Error: Invalid index input.\n";
                cin.clear();
                cin.ignore(10000, '\n');
                break;
            }

            if (arr.deleteAt(index))
                cout << "Line deleted.\n";
            else
                cout << "Error: Invalid index.\n";
            break;

        case 5:
            displayArray(arr);
            break;

        case 6:
            showInfo(arr);
            break;

        case 7:
            arr.clear();
            cout << "Document cleared.\n";
            showInfo(arr);
            break;

        case 8:
            testSet();
            break;

        case 9:
            testMatch();
            break;

        case 10:
            runChecklist();
            break;

        case 0:
            cout << "Exiting test menu.\n";
            break;

        default:
            cout << "Error: Invalid menu choice.\n";
        }

    } while (choice != 0);

    return 0;
}
