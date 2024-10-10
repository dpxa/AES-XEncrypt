#include "../header/ParseInput.h"

int main() {
    std::cout << " ____  ______  __    _    " << std::endl;
    std::cout << "|  _ \\|  _ \\ \\/ /   / \\   " << std::endl;
    std::cout << "| | | | |_) \\  /   / _ \\  " << std::endl;
    std::cout << "| |_| |  __//  \\  / ___ \\ " << std::endl;
    std::cout << "|____/|_|  /_/\\_\\/_/   \\_\\" << std::endl << std::endl;

    std::cout << "Only Ascii characters are encrypted. Non Ascii characters are skipped." << std::endl << std::endl;

    try {
        EncryptedText encryptedText;
        DirectoryDFS ddfs;

        // get path -> ask if key exists -> if so, get path and encrypt status
        //                               -> else generate key
        getValidPath(ddfs);

        if (askYesNo("Is there a key for this path? (yes, no): ")) {
            getValidKeyFile(encryptedText);
            encryptedText.setState(askYesNo("Is this path encrypted? (yes, no): "));
        } else {
            getCreatedKeyFile(encryptedText);
        }

        ddfs.setEncrypted(encryptedText);
        ddfs.performDFS();

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
