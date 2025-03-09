#ifndef DIRECTORYDFS_H
#define DIRECTORYDFS_H

#include "Encrypted.h"
#include <filesystem>
#include <QListWidget>

class DirectoryDFS {
private:
    std::string path;
    EncryptedText encryptedText;

    void processFile(const std::filesystem::path& filePath);

public:
    DirectoryDFS() = default;

    // file or directory
    bool validatePath(const std::string& path);

    void setEncrypted(const EncryptedText& encText) { encryptedText = encText; }

    // start dfs on directory
    void performDFS(std::function<void(const QString&)> fileCallback);
};

#endif // DIRECTORYDFS_H
