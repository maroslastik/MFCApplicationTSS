#pragma once
#include <string>
#include <vector>
#include <windows.h> 

class Image {
public:
    // Constructor
    Image(const std::string& filePath);

    // Getter methods
    const std::string& GetPath() const;
    const std::string& GetFileName() const;
    
private:
    // Data members
    std::string filePath;     // Full path to the image file
    std::string fileName;     // Extracted file name
    
    // Helper method to extract filename from file path
    std::string ExtractFileName(const std::string& path) const;
};