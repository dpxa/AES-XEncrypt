#include "../header/DirectoryDFS.h"
#include <iostream>
#include <algorithm>

// For directories or files
std::string getValidPath(const std::string& prompt) {
    std::string path;

    while (true) {
        std::cout << prompt;
        std::getline(std::cin, path);

        if (std::filesystem::is_regular_file(path) || std::filesystem::is_directory(path)) {
            std::cout << "Valid path" << std::endl;
            return path;
        }
        
        std::cout << "Invalid path. Please try again." << std::endl;
    }
}

// For files only
std::string getValidFilePath(const std::string& prompt) {
    std::string path;

    while (true) {
        std::cout << prompt;
        std::getline(std::cin, path);

        if (std::filesystem::is_regular_file(path)) {
            std::cout << "Valid file" << std::endl;
            return path;
        }
        
        std::cout << "Invalid file path. Please try again." << std::endl;
    }
}

bool askYesNo(const std::string& prompt) {
    std::string response;
    while (true) {
        std::cout << prompt;
        std::getline(std::cin, response);

        std::transform(response.begin(), response.end(), response.begin(), ::tolower);

        if (response == "yes") 
            return true;
        if (response == "no")
            return false;

        std::cout << "Invalid response. Please enter 'yes' or 'no'." << std::endl;
    }
}

int main() {
    try {
        EncryptedText encryptedText;

        // Get file or directory path
        std::string path = getValidPath("Enter a file or directory path: ");

        if (askYesNo("Is there a keyfile for this path? (yes, no): ")) {
            std::string keyfile = getValidFilePath("Enter path to keyfile: ");
            bool isEncrypted = askYesNo("Is this path encrypted? (yes, no): ");

            encryptedText.setKeyAndState(keyfile, isEncrypted);
        } else {
            encryptedText.generateKey();
        }
        
        DirectoryDFS ddfs(path, encryptedText);
        ddfs.performDFS();
    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "Filesystem error: " << e.what() << '\n';
        return EXIT_FAILURE;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
