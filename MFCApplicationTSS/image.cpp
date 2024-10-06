#include "image.h"
#include <algorithm> // For std::find
#include <windows.h> // For handling BITMAP structures
#include <sstream>

// Constructor to initialize the image object
IMAGE::IMAGE(const CString& filePath)
{
    // Assign the passed file path to the class's filePath member variable
    this->filePath = filePath;

    // Call ExtractFileName to extract the file name from the file path
    this->fileName = ExtractFileName(filePath);
}

// Getter methods
const CString& IMAGE::GetPath() const
{
    return filePath;
}

const CString& IMAGE::GetFileName() const
{
    return fileName;
}

// Helper method to extract file name from the file path
CString IMAGE::ExtractFileName(const CString& path) const {
    int pos = path.ReverseFind(_T('\\'));
    if (pos != -1) {
        return path.Mid(pos + 1);  // Extract file name after the last '\'
    }
    return path;  // If no backslash, the path itself is the file name
}