#ifndef DIRECTORYDFS_H
#define DIRECTORYDFS_H

#include "Encrypted.h"
#include <QString>
#include <functional>
#include <filesystem>
#include <chrono>

class DirectoryDFS {
private:
    std::string path;
    EncryptedText encryptedText;
    
    // throughput measurement
    std::chrono::high_resolution_clock::time_point startTime = std::chrono::high_resolution_clock::now();
    size_t totalBytesProcessed = 0;
    int filesProcessed = 0;
    int filesFailed = 0;

    bool processFile(const std::filesystem::path& filePath);

public:
    DirectoryDFS() = default;

    // file or directory
    bool validatePath(const std::string& path);

    void setEncrypted(const EncryptedText& encText) { encryptedText = encText; }

    // start dfs on directory
    void performDFS(std::function<void(const QString&)> fileCallback);

    // throughput measurement functions
    void resetThroughputStats();
    double getCurrentThroughputBytes() const;
    size_t getTotalBytesProcessed() const { return totalBytesProcessed; }
    int getFilesProcessed() const { return filesProcessed; }
    int getFilesFailed() const { return filesFailed; }
    double getElapsedTimeSeconds() const;
};

#endif // DIRECTORYDFS_H
