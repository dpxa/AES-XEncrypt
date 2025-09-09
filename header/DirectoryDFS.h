#ifndef DIRECTORYDFS_H
#define DIRECTORYDFS_H

#include "Encrypted.h"
#include <filesystem>
#include <QListWidget>
#include <chrono>

class DirectoryDFS {
private:
    std::string path;
    EncryptedText encryptedText;
    
    // throughput measurement
    std::chrono::high_resolution_clock::time_point startTime;
    size_t totalBytesProcessed = 0;
    int filesProcessed = 0;

    void processFile(const std::filesystem::path& filePath);

public:
    DirectoryDFS() = default;

    // file or directory
    bool validatePath(const std::string& path);

    void setEncrypted(const EncryptedText& encText) { encryptedText = encText; }

    // start dfs on directory
    void performDFS(std::function<void(const QString&)> fileCallback);

    // throughput measurement functions
    void resetThroughputStats();
    double getCurrentThroughputMBps() const;
    size_t getTotalBytesProcessed() const { return totalBytesProcessed; }
    int getFilesProcessed() const { return filesProcessed; }
    double getElapsedTimeSeconds() const;
};

#endif // DIRECTORYDFS_H
