
#ifndef LINE_H
#define LINE_H

#include <string>

struct Line
{
    std::string text;
    Line* prev;
    Line* next;

    Line(const std::string& lineText = "")
    {
        text = lineText;
        prev = nullptr;
        next = nullptr;
    }
};

#endif
