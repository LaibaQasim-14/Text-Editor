#include "FileManager.h"
#include <fstream>
#include <sstream>

bool FileManager::openFile(Document& document, const std::string& filename)
{
    std::ifstream file(filename);

    if (!file.is_open())
    {
        return false;
    }

    std::ostringstream buffer;
    buffer << file.rdbuf();

    if (file.bad())
    {
        return false;
    }

    return document.loadText(buffer.str());
}

bool FileManager::saveFile(const Document& document, const std::string& filename)
{
    std::ofstream file(filename);

    if (!file.is_open())
    {
        return false;
    }

    file << document.getText();
    file.flush();

    return file.good();
}

bool FileManager::fileExists(const std::string& filename)
{
    std::ifstream file(filename);
    return file.good();
}