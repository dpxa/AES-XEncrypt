#include "../header/DirectoryDFS.h"
#include <iostream>

void DirectoryDFS::performDFS() {
    std::filesystem::path rootPath(directoryPath);

    if (std::filesystem::is_regular_file(rootPath)) {
        mutateFile(rootPath);
    } else {
        performDFSRecursively(rootPath);
    }

    if (!encryptedText.customKey()) {
        std::string keyFilePath;
        std::cout << "No custom key created. Please enter the path to save the encryption key: ";
        std::getline(std::cin, keyFilePath);

        encryptedText.saveKeyToFile(keyFilePath);
    }
}

void DirectoryDFS::performDFSRecursively(const std::filesystem::path& currentDir) {
    // Iterate through entries in the current directory
    for (const auto& entry : std::filesystem::directory_iterator(currentDir)) {
        // output the current file/directory path
        std::cout << entry.path().string() << std::endl;

        if (entry.is_regular_file()) {
            // process regular files
            mutateFile(entry.path());
        } else if (entry.is_directory()) {
            // recurse into subdirectory
            performDFSRecursively(entry.path());
        }
    }
}

void DirectoryDFS::mutateFile(const std::filesystem::path& filePath) {
    encryptedText.setText(filePath.string());    
    (encryptedText.encrypted()) ? encryptedText.decrypt() : encryptedText.encrypt();
    encryptedText.saveToFile(filePath.string());  
}