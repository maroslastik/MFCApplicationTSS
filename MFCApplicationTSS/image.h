#pragma once
#include <string>
#include <vector>
#include <windows.h> 
#include <afxstr.h> 

class IMAGE {
public:
    // Constructor
    IMAGE(const CString& filePath);

    // Getter methods
    const CString& GetPath() const { return filePath; }
    const CString& GetFileName() const { return fileName; }

private:
    // Data members (use CString, not CString&)
    CString filePath;     // Full path to the image file
    CString fileName;     // Extracted file name

    // Helper method to extract the filename from the file path
    CString ExtractFileName(const CString& path) const;
};