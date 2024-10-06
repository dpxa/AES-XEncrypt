#ifndef DIRECTORYDFS_H
#define DIRECTORYDFS_H

#include "Encrypted.h"
#include <filesystem>

class DirectoryDFS {
    private:
    std::string directoryPath;
    EncryptedText encryptedText;

    // encrypt/decrypt file
    void processFile(const std::filesystem::path& filePath);

    public:
    DirectoryDFS(const std::string& dirPath, const EncryptedText& et) : directoryPath(dirPath), encryptedText(et) {}

    // start dfs on directory
    void performDFS();
};

#endif