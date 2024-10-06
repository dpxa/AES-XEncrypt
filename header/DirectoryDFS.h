#ifndef DIRECTORYDFS_H
#define DIRECTORYDFS_H

#include "Encrypted.h"
#include <filesystem>

class DirectoryDFS {
    private:
    std::string directoryPath;
    EncryptedText encryptedText;

    void mutateFile(const std::filesystem::path& filePath);

    public:
    DirectoryDFS(const std::string& dirPath, const EncryptedText& et) : directoryPath(dirPath), encryptedText(et) {}
    void performDFS();
};

#endif