#include "../header/DirectoryDFS.h"
#include <QDateTime>

bool DirectoryDFS::validatePath(const std::string& path) {
    if (!std::filesystem::is_regular_file(path) && !std::filesystem::is_directory(path))
        return false;
    
    this->path = path;
    return true;
}

void DirectoryDFS::performDFS(std::function<void(const QString&)> fileCallback) {
    std::filesystem::path root(path);
    // get rid of / after base
    int rootPathLen = root.parent_path().string().length() + 1;
    
    std::string encStatus = encryptedText.isEncrypted() ? "Decrypted:   " : "Encrypted:   ";

    if (std::filesystem::is_regular_file(root)) {
        processFile(root);
        QString msg = QDateTime::currentDateTime().toString("HH:mm:ss") +
                      "   " + QString::fromStdString(encStatus + root.string().substr(rootPathLen));
        fileCallback(msg);
    } else {
        for (const auto& entry : std::filesystem::recursive_directory_iterator(root)) {
            if (entry.is_regular_file()) {
                processFile(entry.path());

                QString msg = QDateTime::currentDateTime().toString("HH:mm:ss") +
                              "   " + QString::fromStdString(encStatus + entry.path().string().substr(rootPathLen));
                fileCallback(msg);
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
