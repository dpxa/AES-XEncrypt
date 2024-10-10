#ifndef PARSEINPUT_H
#define PARSEINPUT_H

#include "../header/DirectoryDFS.h"
#include <iostream>
#include <algorithm>    // std::transform

// for directories or files
void getValidPath( DirectoryDFS& ddfs) {
    std::string path;

    while (true) {
        std::cout << "Enter a file or directory path to encrypt: ";
        std::getline(std::cin, path);

        // if path is a file or directory
        if (ddfs.validatePath(path))
            break;
        
        std::cout << "Invalid path. Please try again." << std::endl;
    }
}

// for files only
void getValidKeyFile(EncryptedText& encryptedText) {
    std::string path;

    while (true) {
        std::cout << "Enter path to the key: ";
        std::getline(std::cin, path);

        // if path is only a file
        if (encryptedText.validateKeyFile(path))
            break;
        
        std::cout << "Invalid path/key format. Please try again." << std::endl;
    }
}

void getCreatedKeyFile(EncryptedText& encryptedText) {
    std::string path;

    while (true) {
        std::cout << "Enter path to put the key: ";
        std::getline(std::cin, path);

        // if path is only a file
        if (encryptedText.createKeyFile(path))
            break;
        
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