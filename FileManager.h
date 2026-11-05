#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include "Document.h"
#include <string>

class FileManager
{
public:
    bool openFile(Document& document, const std::string& filename);
    bool saveFile(const Document& document, const std::string& filename);
    bool fileExists(const std::string& filename);
};

#endif