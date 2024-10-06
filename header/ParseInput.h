#ifndef PARSEINPUT_H
#define PARSEINPUT_H

#include "../header/DirectoryDFS.h"
#include <iostream>
#include <algorithm>    // std::transform

// for directories or files
std::string getValidPath(const std::string& prompt) {
    std::string path;

    while (true) {
        std::cout << prompt;
        std::getline(std::cin, path);

        // if path is a file or directory
        if (std::filesystem::is_regular_file(path) || std::filesystem::is_directory(path))
            return path;
        
        std::cout << "Invalid path. Please try again." << std::endl;
    }
}

// for files only
std::string getValidFilePath(const std::string& prompt) {
    std::string path;

    while (true) {
        std::cout << prompt;
        std::getline(std::cin, path);

        // if path is only a file
        if (std::filesystem::is_regular_file(path))
            return path;
        
        std::cout << "Invalid file path. Please try again." << std::endl;
    }
}

// for validating parent directory
std::string getValidParentDirectory(const std::string& prompt) {
    std::string path;

    while (true) {
        std::cout << prompt;
        std::getline(std::cin, path);

        std::filesystem::path filePath(path);
        std::filesystem::path absPath = std::filesystem::absolute(filePath);
        std::filesystem::path parentDir = absPath.parent_path();

        // if path is only a file
        if (std::filesystem::is_directory(parentDir))
            return path;
        
        std::cout << "Invalid file path. Please try again." << std::endl;
    }
}

bool askYesNo(const std::string& prompt) {
    std::string response;

    while (true) {
        std::cout << prompt;
        std::getline(std::cin, response);

        // turn response to lowercase
        std::transform(response.begin(), response.end(), response.begin(), ::tolower);

        if (response == "yes") 
            return true;
        if (response == "no")
            return false;

        std::cout << "Invalid response. Please enter 'yes' or 'no'." << std::endl;
    }
}

#endif