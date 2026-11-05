#ifndef MENU_H
#define MENU_H

#include "Editor.h"

class Menu
{
private:
    Editor editor;
    bool running;

    // Main menu
    void displayMenu();
    void handleNew();
    void handleOpen();
    void handleExit();

    // Editing menu
    void editingMenu();

    // Editing operations
    void handleInsert();
    void handleDelete();
    void handleSearch();
    void handleReplace();
    void handleUndo();
    void handleRedo();
    void handleDisplay();
    void handleInfo();

    // Saving operations
    void handleSave();
    void handleSaveAs();

public:
    Menu();
    void run();
};

#endif