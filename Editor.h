#ifndef EDITOR_H
#define EDITOR_H

#include <string>
#include <cstddef>

#include "Document.h"
#include "UndoManager.h"
#include "FileManager.h"
#include "TextProcessor.h"
#include "EditOperation.h"
#include "DynamicArray.h"
#include "Match.h"

class Editor
{
private:

    // Main components
    Document document;
    UndoManager undoManager;
    FileManager fileManager;

    // Current file information
    std::string currentFilename;
    bool modified;

    // Used to group Replace All operations
    std::size_t nextGroupId;

    // Checks whether unsaved changes should be saved or discarded
    bool confirmSaveIfModified();

public:

    // Constructor
    Editor();

    // Get document for Menu
    Document& getDocument();

    // Document and file operations
    bool newDocument();
    bool open(const std::string& filename);
    bool save();
    bool saveAs(const std::string& filename);

    // Editing operations
    bool insert(int line, int col, const std::string& text);
    bool remove(int line, int col, int length);

    // Search and replace
    bool search(const std::string& searchText,
                DynamicArray<Match>& results);

    bool replace(const Match& position,
                 const std::string& searchText,
                 const std::string& replacementText);

    bool replaceAll(const std::string& searchText,
                    const std::string& replacementText);

    // Undo
    bool undo();

    // Information and exit
    void showInfo() const;
    bool exit();
};

#endif