#include "../header/DirectoryDFS.h"

bool DirectoryDFS::validatePath(const std::string& path) {
    if (!std::filesystem::is_regular_file(path) && !std::filesystem::is_directory(path))
        return false;
    
    this->path = path;
    return true;
}

void DirectoryDFS::performDFS(QListWidget* fileListWidget) {
    std::filesystem::path root(path);

    std::string encStatus = encryptedText.isEncrypted() ? "Decrypted:   " : "Encrypted:   ";

    if (std::filesystem::is_regular_file(root)) {
        processFile(root);
        fileListWidget->insertItem(0, QString::fromStdString(encStatus + root.string()));
    } else {
        for (const auto& entry : std::filesystem::recursive_directory_iterator(root)) {
            if (entry.is_regular_file()) {
                processFile(entry.path());
                fileListWidget->insertItem(0, QString::fromStdString(encStatus + entry.path().string()));
            }
        }
    }
}

void DirectoryDFS::processFile(const std::filesystem::path& filePath) {
    try {
        encryptedText.setData(filePath.string());
        encryptedText.process();
        encryptedText.saveData(filePath.string());

    } catch (const std::runtime_error& e) {
        qDebug() << "Error processing file: " << filePath.string() << '\n'
                  << "Runtime error: " << e.what() << '\n';
    }
}
