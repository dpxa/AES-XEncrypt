#include "../header/DirectoryDFS.h"
#include <iostream>

void DirectoryDFS::performDFS() {
    if (!encryptedText.customKey()) {
        std::string keyFilePath;
        std::cout << "No custom key created. Please enter the path to save the encryption key: ";
        std::getline(std::cin, keyFilePath);
        encryptedText.saveKeyToFile(keyFilePath);
    }

    std::filesystem::path rootPath(directoryPath);

    if (std::filesystem::is_regular_file(rootPath)) {
        mutateFile(rootPath);
    } else {
        for (const auto& entry : std::filesystem::recursive_directory_iterator(rootPath)) {
            if (entry.is_regular_file()) {
                std::cout << entry.path().string() << std::endl;
                mutateFile(entry.path());
            }
        }
    }
}

void DirectoryDFS::mutateFile(const std::filesystem::path& filePath) {
    encryptedText.setText(filePath.string());
    (encryptedText.encrypted()) ? encryptedText.decrypt() : encryptedText.encrypt();
    encryptedText.saveToFile(filePath.string());
}
