
#include "Menu.h"
#include "InputHelper.h"

#include <iostream>
#include <string>
#include <limits>

using namespace std;

// Constructor
Menu::Menu()
    : running(true)
{
}

// Main Menu
void Menu::displayMenu()
{
    cout << "\n========== TEXT EDITOR ==========\n";
    cout << "1. New Document\n";
    cout << "2. Open Document\n";
    cout << "3. Exit\n";
    cout << "=================================\n";
}

// Create New Document
void Menu::handleNew()
{
    if (editor.newDocument())
    {
        cout << "\nNew document created successfully.\n";
        editingMenu();
    }
    else
    {
        cout << "Unable to create new document.\n";
    }
}

// Open Existing Document
void Menu::handleOpen()
{
    string filename = InputHelper::readLine(
        "Enter filename: "
    );

    if (filename.empty())
    {
        cout << "Filename cannot be empty.\n";
        return;
    }

    if (editor.open(filename))
    {
        cout << "\nDocument opened successfully.\n";

        if (editor.getDocument().getCharCount() > 0)
            handleDisplay();

        editingMenu();
    }
    else
    {
        cout << "Unable to open document.\n";
    }
}

// Editing Menu

void Menu::editingMenu()
{
    while (true)
    {
        cout << "\n========== TEXT EDITOR ==========\n";

        bool hasText = editor.getDocument().getCharCount() > 0;
        bool canUndo = editor.canUndo();
        bool canRedo = editor.canRedo();

        int option = 1;

        int insertChoice = -1;
        int deleteChoice = -1;
        int searchChoice = -1;
        int replaceChoice = -1;
        int undoChoice = -1;
        int redoChoice = -1;
        int displayChoice = -1;
        int infoChoice = -1;
        int saveChoice = -1;
        int saveAsChoice = -1;
        int backChoice = -1;

        // Insert is always available
        insertChoice = option;
        cout << option++ << ". Insert Text\n";

        // Show editing options only when document has text
        if (hasText)
        {
            deleteChoice = option;
            cout << option++ << ". Delete Text\n";

            searchChoice = option;
            cout << option++ << ". Search Text\n";

            replaceChoice = option;
            cout << option++ << ". Replace Text\n";

            displayChoice = option;
            cout << option++ << ". Display Document\n";

            infoChoice = option;
            cout << option++ << ". Document Information\n";
        }

        // Undo option
        if (canUndo)
        {
            undoChoice = option;
            cout << option++ << ". Undo\n";
        }

        // Redo option
        if (canRedo)
        {
            redoChoice = option;
            cout << option++ << ". Redo\n";
        }

        // Save options
        saveChoice = option;
        cout << option++ << ". Save\n";

        saveAsChoice = option;
        cout << option++ << ". Save As\n";

        // Back option
        backChoice = option;
        cout << option++ << ". Back to Main Menu\n";

        cout << "=================================\n";

        int choice;

        cout << "Enter your choice: ";

        if (!(cin >> choice))
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            cout << "Invalid input! Enter a number.\n";
            continue;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (choice == insertChoice)
        {
            handleInsert();
        }
        else if (choice == deleteChoice && deleteChoice != -1)
        {
            handleDelete();
        }
        else if (choice == searchChoice && searchChoice != -1)
        {
            handleSearch();
        }
        else if (choice == replaceChoice && replaceChoice != -1)
        {
            handleReplace();
        }
        else if (choice == displayChoice && displayChoice != -1)
        {
            handleDisplay();
        }
        else if (choice == infoChoice && infoChoice != -1)
        {
            handleInfo();
        }
        else if (choice == undoChoice && undoChoice != -1)
        {
            handleUndo();
        }
        else if (choice == redoChoice && redoChoice != -1)
        {
            handleRedo();
        }
        else if (choice == saveChoice)
        {
            handleSave();
        }
        else if (choice == saveAsChoice)
        {
            handleSaveAs();
        }
        else if (choice == backChoice)
        {
            if (editor.exit())
            {
                return;
            }
        }
        else
        {
            cout << "Invalid choice! Please try again.\n";
        }
    }
}

// Insert Text
void Menu::handleInsert()
{
    cout << "\n------ Writing Mode ------\n";
    cout << "Enter your text below.\n";
    cout << "Type :done on a separate line to finish.\n\n";

    string block;
    string line;

    bool firstLine = true;

    while (true)
    {
        line = InputHelper::readLine("> ");

        if (line == ":done")
            break;

        if (!firstLine)
            block += '\n';

        block += line;
        firstLine = false;
    }

    if (editor.insertBlockAtEnd(block, !firstLine))
    {
        cout << "\nText inserted successfully!\n";
    }
    else
    {
        cout << "\nNo text was inserted.\n";
    }
}
// Delete Text
void Menu::handleDelete()
{
    int line = InputHelper::readInt(
        "Enter line number: "
    );

    int col = InputHelper::readInt(
        "Enter starting column: "
    );

    int length = InputHelper::readInt(
        "Enter number of characters to delete: "
    );

    line--;
    col--;

    if (editor.remove(line, col, length))
        cout << "Text deleted successfully.\n";
    else
        cout << "Unable to delete text. Check your inputs.\n";
}

// Search Text
void Menu::handleSearch()
{
    string searchText = InputHelper::readLine(
        "Enter text to search: "
    );

    DynamicArray<Match> results;

    if (!editor.search(searchText, results))
    {
        cout << "Search failed.\n";
        return;
    }

    if (results.getSize() == 0)
    {
        cout << "No matches found.\n";
        return;
    }

    cout << "\nMatches found: " << results.getSize() << '\n';

    for (int i = 0; i < results.getSize(); i++)
    {
        Match* match = results.getAt(i);

        if (match != nullptr)
        {
            cout << "Line: " << match->line + 1
                 << ", Column: " << match->col + 1
                 << '\n';
        }
    }
}

// Replace Text
void Menu::handleReplace()
{
    cout << "\n1. Replace one occurrence\n";
    cout << "2. Replace all occurrences\n";

    int choice = InputHelper::readInt("Enter choice: ");

    string searchText = InputHelper::readLine(
        "Enter text to find: "
    );

    string replacementText = InputHelper::readLine(
        "Enter replacement text: "
    );

    if (choice == 1)
    {
        DynamicArray<Match> results;

        if (!editor.search(searchText, results))
        {
            cout << "Search failed.\n";
            return;
        }

        if (results.getSize() == 0)
        {
            cout << "No matches found.\n";
            return;
        }

        for (int i = 0; i < results.getSize(); i++)
        {
            Match* match = results.getAt(i);

            if (match != nullptr)
            {
                cout << i + 1
                     << ". Line: " << match->line + 1
                     << ", Column: " << match->col + 1
                     << '\n';
            }
        }

        int occurrence = InputHelper::readInt(
            "Select occurrence: "
        );

        if (occurrence < 1 || occurrence > results.getSize())
        {
            cout << "Invalid occurrence.\n";
            return;
        }

        Match* selected = results.getAt(occurrence - 1);

        if (selected != nullptr &&
            editor.replace(*selected, searchText, replacementText))
        {
            cout << "Text replaced successfully.\n";
        }
        else
        {
            cout << "Replacement failed.\n";
        }
    }
    else if (choice == 2)
    {
        if (editor.replaceAll(searchText, replacementText))
            cout << "All occurrences replaced successfully.\n";
        else
            cout << "Replace all failed.\n";
    }
    else
    {
        cout << "Invalid choice.\n";
    }
}

// Undo
void Menu::handleUndo()
{
    if (editor.undo())
        cout << "Last operation undone successfully.\n";
    else
        cout << "Nothing to undo.\n";
}
void Menu::handleRedo()
{
    if (editor.redo())
    {
        cout << "Redo successful!\n";
    }
    else
    {
        cout << "Nothing to redo.\n";
    }
}

// Display Document
void Menu::handleDisplay()
{
    cout << "\n========== DOCUMENT ==========\n";

    editor.getDocument().display(cout);

    cout << "==============================\n";
}

// Document Information
void Menu::handleInfo()
{
    editor.showInfo();
}

// Save
void Menu::handleSave()
{
    // If document has no filename, ask for one
    if (!editor.hasFilename())
    {
        cout << "\nThis document has not been saved yet.\n";
        handleSaveAs();
        return;
    }

    if (editor.save())
        cout << "Document saved successfully.\n";
    else
        cout << "Unable to save document.\n";
}

// Save As
void Menu::handleSaveAs()
{
    string filename = InputHelper::readLine(
        "Enter filename: "
    );

    if (filename.empty())
    {
        cout << "Filename cannot be empty.\n";
        return;
    }

    if (editor.saveAs(filename))
        cout << "Document saved successfully.\n";
    else
        cout << "Unable to save document.\n";
}

// Exit
void Menu::handleExit()
{
    if (editor.exit())
    {
        cout << "Exiting Text Editor...\n";
        running = false;
    }
    else
    {
        cout << "Exit cancelled.\n";
    }
}

// Main Program Loop
void Menu::run()
{
    while (running)
    {
        displayMenu();

        int choice = InputHelper::readInt(
            "Enter your choice: "
        );

        switch (choice)
        {
            case 1:
                handleNew();
                break;

            case 2:
                handleOpen();
                break;

            case 3:
                handleExit();
                break;

            default:
                cout << "Invalid choice. Select 1, 2 or 3.\n";
        }
    }
}
