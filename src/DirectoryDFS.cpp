#include "../header/DirectoryDFS.h"
#include <QDateTime>

bool DirectoryDFS::validatePath(const std::string& path) {
    if (!std::filesystem::is_regular_file(path) && !std::filesystem::is_directory(path))
        return false;
    
    this->path = path;
    return true;
}

void DirectoryDFS::resetThroughputStats() {
    startTime = std::chrono::high_resolution_clock::now();
    totalBytesProcessed = 0;
    filesProcessed = 0;
    filesFailed = 0;
}

double DirectoryDFS::getCurrentThroughputBytes() const {
    auto elapsed = getElapsedTimeSeconds();
    if (elapsed > 0.0) {
        return totalBytesProcessed / elapsed;
    }
    return 0.0;
}

double DirectoryDFS::getElapsedTimeSeconds() const {
    auto currentTime = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime);
    return elapsed.count() / 1000.0;
}

void DirectoryDFS::performDFS(std::function<void(const QString&)> fileCallback) {
    std::filesystem::path root(path);
    // get rid of / after base
    int rootPathLen = root.parent_path().string().length() + 1;
    
    std::string encStatus = encryptedText.isEncrypted() ? "Decrypted:   " : "Encrypted:   ";

    resetThroughputStats();

    try {
        auto reportProcessedFile = [&](const std::filesystem::path& filePath) {
            std::error_code error;
            const auto fileSize = std::filesystem::file_size(filePath, error);
            if (error) {
                filesFailed++;
                qDebug() << "Filesystem error for file: " << filePath.string().c_str()
                         << '\n' << error.message().c_str() << '\n';
                return;
            }
            if (!processFile(filePath)) {
                filesFailed++;
                return;
            }

            totalBytesProcessed += fileSize;
            filesProcessed++;

            QString msg = QDateTime::currentDateTime().toString("HH:mm:ss") +
                          "   " + QString::fromStdString(encStatus + filePath.string().substr(rootPathLen));
            fileCallback(msg);
        };

        if (std::filesystem::is_regular_file(root)) {
            reportProcessedFile(root);
        } else {
            for (const auto& entry : std::filesystem::recursive_directory_iterator(root)) {
                std::error_code error;
                const bool isRegularFile = entry.is_regular_file(error);
                if (error) {
                    filesFailed++;
                    continue;
                }
                if (isRegularFile) {
                    reportProcessedFile(entry.path());
                }
            }
        }
    } catch (const std::filesystem::filesystem_error& e) {
        qDebug() << "Filesystem error while processing: " << e.what() << '\n';
    }
}

bool DirectoryDFS::processFile(const std::filesystem::path& filePath) {
    try {
        if (!encryptedText.setData(filePath.string())) {
            throw std::runtime_error("Failed to read file: " + filePath.string());
        }
        if (!encryptedText.process()) {
            throw std::runtime_error("Encryption/decryption failed for file: " + filePath.string());
        }
        if (!encryptedText.saveData(filePath.string())) {
            throw std::runtime_error("Failed to write encrypted/decrypted file: " + filePath.string());
        }
        return true;

    } catch (const std::runtime_error& e) {
        qDebug() << "Error processing file: " << filePath.string() << '\n'
                  << "Runtime error: " << e.what() << '\n';
        return false;
    }
}
