#include "Image.h"
#include <algorithm> // For std::find
#include <windows.h> // For handling BITMAP structures
#include <sstream>

// Constructor to initialize the image object
Image::Image(const std::string& filePath) : filePath(filePath), fileName(ExtractFileName(filePath)) 
{

}

// Getter methods
const std::string& Image::GetPath() const 
{
    return filePath;
}

const std::string& Image::GetFileName() const 
{
    return fileName;
}

// Helper method to extract file name from the file path
std::string Image::ExtractFileName(const std::string& path) const 
{
    size_t pos = path.find_last_of("/\\");
    if (pos != std::string::npos) 
    {
        return path.substr(pos + 1);
    }
    return path; 
}