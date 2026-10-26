#include "Document.h"
#include "TextUtils.h" //laiba ki file 
#include <ostream>

using namespace std;

// Constructor: every document starts with one empty line.
Document::Document()
{
    lines.insertAt(0, "");
}

// Insert a new line.
bool Document::insertLine(int index, const string& text)
{
    if (index < 0 || index > getLineCount())
        return false;

    return lines.insertAt(index, text);
}

// Replace the text of an existing line.
bool Document::setLine(int index, const string& text)
{
    Line* current = lines.getAt(index);

    if (current == nullptr)
        return false;

    current->text = text;
    return true;
}

// Delete a line, keeping at least one line in the document.
bool Document::deleteLine(int index)
{
    Line* current = lines.getAt(index);

    if (current == nullptr)
        return false;

    // Never remove the last line.
    if (getLineCount() == 1)
    {
        current->text = "";
        return true;
    }

    return lines.deleteAt(index);
}

// Delete multiple consecutive lines.
bool Document::deleteLines(int start, int count)
{
    if (start < 0 || count <= 0 ||
        start >= getLineCount() ||
        count > getLineCount() - start)
    {
        return false;
    }

    for (int i = 0; i < count; i++)
    {
        if (!lines.deleteAt(start))
            return false;
    }

    // Restore one empty line if all lines were deleted.
    if (lines.isEmpty())
        lines.insertAt(0, "");

    return true;
}

// Get a line without allowing the caller to modify it.
const Line* Document::getLine(int index) const
{
    return lines.getAt(index);
}

// Return the number of lines.
int Document::getLineCount() const
{
    return lines.getSize();
}

// Count characters, excluding newline separators.
int Document::getCharCount() const
{
    int total = 0;

    for (int i = 0; i < getLineCount(); i++)
    {
        const Line* current = getLine(i);

        if (current != nullptr)
            total += static_cast<int>(current->text.length());
    }

    return total;
}

// Empty means one empty line.
bool Document::isEmpty() const
{
    return getLineCount() == 1 &&
           getLine(0)->text.empty();
}

// Split a line at a character position.
bool Document::splitLine(int lineIndex, int charPosition)
{
    Line* current = lines.getAt(lineIndex);

    if (current == nullptr ||
        charPosition < 0 ||
        charPosition > static_cast<int>(current->text.length()))
    {
        return false;
    }

    string left = current->text.substr(0, charPosition);
    string right = current->text.substr(charPosition);

    current->text = left;

    return lines.insertAt(lineIndex + 1, right);
}

// Display the entire document.
void Document::display(ostream& out) const
{
    lines.display(out);
}

// Display lines from startLine to endLine, inclusive.
void Document::displayRange(
    ostream& out, int startLine, int endLine) const
{
    if (startLine < 0 ||
        endLine < startLine ||
        endLine >= getLineCount())
    {
        return;
    }

    for (int i = startLine; i <= endLine; i++)
    {
        const Line* current = getLine(i);

        out << i << ": " << current->text << '\n';
    }
}

// Remove all lines and leave one empty line.
void Document::clear()
{
    lines.clear();
    lines.insertAt(0, "");
}

// Return the complete document, preserving newline characters.
string Document::getText() const
{
    string result;

    for (int i = 0; i < getLineCount(); i++)
    {
        const Line* current = getLine(i);

        result += current->text;

        if (i < getLineCount() - 1)
            result += '\n';
    }

    return result;
}

// Load text by processing characters manually.
bool Document::loadText(const string& text)
{
    clear();

    for (size_t i = 0; i < text.length(); i++)
    {
        if (text[i] == '\n')
        {
            if (!insertLine(getLineCount(), ""))
            {
                clear();
                return false;
            }
        }
        else
        {
            Line* current = lines.getAt(getLineCount() - 1);

            if (current == nullptr)
            {
                clear();
                return false;
            }

            current->text += text[i];
        }
    }

    return true;
}

// Insert text into an existing line.
bool Document::insertText(
    int lineIndex, int charPosition, const string& text)
{
    Line* current = lines.getAt(lineIndex);

    if (current == nullptr ||
        charPosition < 0 ||
        charPosition > static_cast<int>(current->text.length()))
    {
        return false;
    }

    return insertInString(
        current->text, charPosition, text);
}

// Delete one character.
bool Document::deleteChar(int lineIndex, int charPosition)
{
    return deleteRange(lineIndex, charPosition, 1);
}

// Delete a range of characters from one line.
bool Document::deleteRange(
    int lineIndex, int startPosition, int length)
{
    Line* current = lines.getAt(lineIndex);

    if (current == nullptr ||
        startPosition < 0 ||
        length <= 0 ||
        startPosition > static_cast<int>(current->text.length()) ||
        length > static_cast<int>(current->text.length()) -
                 startPosition)
    {
        return false;
    }

    string deletedText;

    return deleteInString(
        current->text, startPosition, length, deletedText);
}

// Delete the word at the given character position.
bool Document::deleteWord(int lineIndex, int charPosition)
{
    Line* current = lines.getAt(lineIndex);

    if (current == nullptr || charPosition < 0)
        return false;

    string deletedText;
    size_t startCol = 0;

    return deleteWordInString(
        current->text, charPosition, deletedText, startCol);
}
