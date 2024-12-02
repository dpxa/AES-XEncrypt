#include "../header/DirectoryDFS.h"

bool DirectoryDFS::validatePath(const std::string& path) {
    if (!std::filesystem::is_regular_file(path) && !std::filesystem::is_directory(path))
        return false;
    
    this->path = path;
    return true;
}

void DirectoryDFS::performDFS(QListWidget* fileListWidget) {
    std::filesystem::path root(path);

    std::string encStatus = encryptedText.isEncrypted() ? "Encrypted:   " : "Decrypted:   ";

    // no recursion if root is a file
    if (std::filesystem::is_regular_file(root)) {
        processFile(root);
        fileListWidget->addItem(QString::fromStdString(encStatus + root.string()));
    // recursion if root is a directory
    } else {
        for (const auto& entry : std::filesystem::recursive_directory_iterator(root)) {
            if (entry.is_regular_file()) {
                processFile(entry.path());
                fileListWidget->addItem(QString::fromStdString(encStatus + entry.path().string()));
            }
        }
    }
}

void DirectoryDFS::processFile(const std::filesystem::path& filePath) {
    try {
        encryptedText.setText(filePath.string());
        encryptedText.process();
        encryptedText.saveTextToFile(filePath.string());

    } catch (const std::runtime_error& e) {
        qDebug() << "Error processing file: " << filePath.string() << '\n'
                  << "Runtime error: " << e.what() << '\n';
    }
}
