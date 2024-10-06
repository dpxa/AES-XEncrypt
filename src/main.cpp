#include "../header/ParseInput.h"

int main() {
    std::cout << " ____  ______  __    _    " << std::endl;
    std::cout << "|  _ \\|  _ \\ \\/ /   / \\   " << std::endl;
    std::cout << "| | | | |_) \\  /   / _ \\  " << std::endl;
    std::cout << "| |_| |  __//  \\  / ___ \\ " << std::endl;
    std::cout << "|____/|_|  /_/\\_\\/_/   \\_\\" << std::endl << std::endl;

    try {
        EncryptedText encryptedText;

        // get path -> ask if key exists -> if so, get path and encrypt status
        //                               -> else generate key
        std::string path = getValidPath("Enter a file or directory path: ");

        if (askYesNo("Is there a key for this path? (yes, no): ")) {
            std::string keyfile = getValidFilePath("Enter path to key: ");
            bool isEncrypted = askYesNo("Is this path encrypted? (yes, no): ");

            encryptedText.setKeyAndState(keyfile, isEncrypted);
        } else {
            encryptedText.generateKey();
            std::string keyFilePath = getValidParentDirectory("Please enter the path to save the encryption key: ");
            encryptedText.saveKeyToFile(keyFilePath);
        }
        
        DirectoryDFS ddfs(path, encryptedText);
        ddfs.performDFS();

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
