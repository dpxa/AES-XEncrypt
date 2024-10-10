#include "../header/DirectoryDFS.h"

bool DirectoryDFS::validatePath(const std::string& filePath) {
    if (!std::filesystem::is_regular_file(filePath) && !std::filesystem::is_directory(filePath))
        return false;
    
    directoryPath = filePath;
    return true;
}

void DirectoryDFS::setEncrypted(const EncryptedText& et) {
    encryptedText = et;
}

void DirectoryDFS::performDFS(QListWidget* fileListWidget) {
    std::filesystem::path rootPath(directoryPath);

    // no recursion if root is a file
    if (std::filesystem::is_regular_file(rootPath)) {
        processFile(rootPath);
        fileListWidget->addItem(QString::fromStdString(rootPath.string()));
    // recursion if root is a directory
    // already made sure rootPath is a file or directory in main
    } else {
        for (const auto& entry : std::filesystem::recursive_directory_iterator(rootPath)) {
            if (entry.is_regular_file()) {
                processFile(entry.path());
                fileListWidget->addItem(QString::fromStdString(entry.path().string()));
            }
        }
    }
}

void DirectoryDFS::processFile(const std::filesystem::path& filePath) {
    // read in entire contents of file into encryptedText
    encryptedText.setText(filePath.string());
    // process the text
    encryptedText.process();
    // write the processed text to the same file
    encryptedText.saveTextToFile(filePath.string());
}
