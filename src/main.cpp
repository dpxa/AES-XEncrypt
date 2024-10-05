#include "../header/Encrypted.h"
#include "../header/methods.h"

int main() {
    try {
        std::string file;
        std::cout << "Enter file to encrypt: ";
        std::getline(std::cin, file);

        // Read text from the file and create an Encrypted object
        Encrypted encryptedData(get_text(file), false);
        encryptedData.encrypt();
        encryptedData.save("encrypted.txt");
        std::cout << "Encrypted data saved to encrypted.txt.\n";

        // Decrypt the data and save the output
        encryptedData.decrypt();
        encryptedData.save("output.txt");
        std::cout << "Decrypted data saved to output.txt.\n";

        // Save the cipher key
        encryptedData.saveKey("cipher.txt");
        std::cout << "Cipher key saved to cipher.txt.\n";
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
