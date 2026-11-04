
#include "Editor.h"

#include <iostream>
#include <limits>

// Constructor
Editor::Editor()
    : currentFilename(""),
      modified(false),
      nextGroupId(1)
{
}

// Ask the user what to do with unsaved changes
bool Editor::confirmSaveIfModified()
{
    if (!modified)
        return true;

    char choice;

    std::cout << "\nYou have unsaved changes.\n";
    std::cout << "Save changes? (y = yes, n = no, c = cancel): ";
    std::cin >> choice;

    std::cin.ignore(
        std::numeric_limits<std::streamsize>::max(), '\n'
    );

    if (choice == 'y' || choice == 'Y')
    {
        if (currentFilename.empty())
        {
            std::string filename;

            std::cout << "Enter filename: ";
            std::getline(std::cin, filename);

            return saveAs(filename);
        }

        return save();
    }

    if (choice == 'n' || choice == 'N')
        return true;

    // Cancel the requested operation
    return false;
}

// Create a new empty document
bool Editor::newDocument()
{
    if (!confirmSaveIfModified())
        return false;

    document.clear();
    undoManager.clear();

    currentFilename.clear();
    modified = false;

    return true;
}

// Open an existing file
bool Editor::open(const std::string& filename)
{
    if (!confirmSaveIfModified())
        return false;

    if (filename.empty())
        return false;

    if (!fileManager.openFile(document, filename))
        return false;

    currentFilename = filename;
    modified = false;

    // Old document operations must not be undoable
    undoManager.clear();

    return true;
}

// Save changes to the current file
bool Editor::save()
{
    if (currentFilename.empty())
        return false;

    if (!fileManager.saveFile(document, currentFilename))
        return false;

    modified = false;

    return true;
}

// Save document using a new filename
bool Editor::saveAs(const std::string& filename)
{
    if (filename.empty())
        return false;

    if (!fileManager.saveFile(document, filename))
        return false;

    currentFilename = filename;
    modified = false;

    return true;
}

// Insert text into the document
bool Editor::insert(int line, int col, const std::string& text)
{
    if (text.empty())
        return false;

    // Step 1: Change the document
    if (!document.insertText(line, col, text))
        return false;

    // Step 2: Record the operation
    EditOperation op;

    op.type = OperationType::INSERT_TEXT;
    op.line = static_cast<std::size_t>(line);
    op.col = static_cast<std::size_t>(col);
    op.newText = text;

    undoManager.record(op);

    // Step 3: Mark document as modified
    modified = true;

    return true;
}

// Remove a range of text from the document
bool Editor::remove(int line, int col, int length)
{
    if (length <= 0)
        return false;

    const Line* current = document.getLine(line);

    if (current == nullptr ||
        col < 0 ||
        col > static_cast<int>(current->text.length()) ||
        length > static_cast<int>(current->text.length()) - col)
    {
        return false;
    }

    // Save deleted text for undo
    std::string deletedText =
        current->text.substr(
            static_cast<std::size_t>(col),
            static_cast<std::size_t>(length)
        );

    // Step 1: Change the document
    if (!document.deleteRange(line, col, length))
        return false;

    // Step 2: Record the operation
    EditOperation op;

    op.type = OperationType::DELETE_TEXT;
    op.line = static_cast<std::size_t>(line);
    op.col = static_cast<std::size_t>(col);
    op.oldText = deletedText;

    undoManager.record(op);

    // Step 3: Mark document as modified
    modified = true;

    return true;
}

// Search for all occurrences of text
bool Editor::search(
    const std::string& searchText,
    DynamicArray<Match>& results)
{
    return TextProcessor::searchAll(
        document, searchText, results
    );
}

// Replace one occurrence
bool Editor::replace(
    const Match& position,
    const std::string& searchText,
    const std::string& replacementText)
{
    if (searchText.empty())
        return false;

    if (searchText == replacementText)
        return false;

    ReplacementRecord record;

    // Step 1: Change the document
    if (!TextProcessor::replaceOccurrence(
            document,
            position,
            searchText,
            replacementText,
            record))
    {
        return false;
    }

    // Step 2: Record the operation
    EditOperation op;

    op.type = OperationType::REPLACE;
    op.line = static_cast<std::size_t>(record.position.line);
    op.col = static_cast<std::size_t>(record.position.col);
    op.oldText = record.oldText;
    op.newText = record.newText;

    undoManager.record(op);

    // Step 3: Mark document as modified
    modified = true;

    return true;
}

// Replace all occurrences
bool Editor::replaceAll(
    const std::string& searchText,
    const std::string& replacementText)
{
    if (searchText.empty() || searchText == replacementText)
        return false;

    DynamicArray<ReplacementRecord> records;

    // Step 1: TextProcessor changes the document
    if (!TextProcessor::replaceAll(
            document, searchText, replacementText, records))
    {
        return false;
    }

    // No occurrences were replaced
    if (records.getSize() == 0)
        return true;

    // One group ID makes all replacements undo together
    std::size_t group = nextGroupId++;

    if (nextGroupId == 0)
        nextGroupId = 1;

    // Step 2: Record every replacement
    for (int i = 0; i < records.getSize(); i++)
    {
        ReplacementRecord* record = records.getAt(i);

        if (record == nullptr)
            continue;

        EditOperation op;

        op.type = OperationType::REPLACE;
        op.line = static_cast<std::size_t>(
            record->position.line
        );
        op.col = static_cast<std::size_t>(
            record->position.col
        );
        op.oldText = record->oldText;
        op.newText = record->newText;
        op.groupId = group;

        undoManager.record(op);
    }

    // Step 3: Mark document as modified
    modified = true;

    return true;
}

// Undo the last editing operation
bool Editor::undo()
{
    if (!undoManager.undo(document))
        return false;

    modified = true;

    return true;
}

// Display document information
void Editor::showInfo() const
{
    std::cout << "\n===== DOCUMENT INFORMATION =====\n";

    if (currentFilename.empty())
        std::cout << "Filename: Untitled\n";
    else
        std::cout << "Filename: " << currentFilename << '\n';

    std::cout << "Total lines: "
              << document.getLineCount() << '\n';

    std::cout << "Total characters: "
              << document.getCharCount() << '\n';

    std::cout << "Modified: "
              << (modified ? "Yes" : "No") << '\n';

    std::cout << "Undo operations available: "
              << undoManager.getSize() << '\n';

    std::cout << "================================\n";
}

// Check for unsaved changes before exiting
bool Editor::exit()
{
    return confirmSaveIfModified();
}
