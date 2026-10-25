
#ifndef DOCUMENT_H
#define DOCUMENT_H

#include "DoublyLinkedList.h"
#include <string>
#include <iosfwd>

class Document
{
private:
    DoublyLinkedList lines;

public:
    Document();

    bool insertLine(int index, const std::string& text);
    bool setLine(int index, const std::string& text);
    bool deleteLine(int index);
    bool deleteLines(int start, int count);

    const Line* getLine(int index) const;

    int getLineCount() const;
    int getCharCount() const;
    bool isEmpty() const;

    bool splitLine(int lineIndex, int charPosition);

    void display(std::ostream& out) const;
    void displayRange(std::ostream& out, int startLine, int endLine) const;
    void clear();

    std::string getText() const;
    bool loadText(const std::string& text);

    bool insertText(int lineIndex, int charPosition,
                    const std::string& text);
    bool deleteChar(int lineIndex, int charPosition);
    bool deleteRange(int lineIndex, int startPosition, int length);
    bool deleteWord(int lineIndex, int charPosition);
};

#endif
