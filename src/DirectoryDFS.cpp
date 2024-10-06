#include "../header/DirectoryDFS.h"
#include <iostream>

void DirectoryDFS::performDFS() {
    std::filesystem::path rootPath(directoryPath);

    // no recursion if root is a file
    if (std::filesystem::is_regular_file(rootPath)) {
        processFile(rootPath);
    // recursion if root is a directory
    // a;ready made sure rootPath is a file or directory in main
    } else {
        for (const auto& entry : std::filesystem::recursive_directory_iterator(rootPath)) {
            if (entry.is_regular_file()) {

                // output each files full name
                std::cout << entry.path().string() << std::endl;
                processFile(entry.path());
            }
        }
    }
}

void DirectoryDFS::processFile(const std::filesystem::path& filePath) {
    // read in entire contents of file into encryptedText
    encryptedText.setText(filePath.string());
    // process the text
    (encryptedText.encrypted()) ? encryptedText.decrypt() : encryptedText.encrypt();
    // write the processed text to the same file
    encryptedText.saveTextToFile(filePath.string());
}
