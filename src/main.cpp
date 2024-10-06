#include "../header/Encrypted.h"

int main() {
    try {
        std::string file;
        std::cout << "Enter file to encrypt: ";
        std::getline(std::cin, file);

        // Read text from the file and create an EncryptedText object
        EncryptedText encryptedData(file, false);
        encryptedData.encrypt();  // Renamed from encrypt
        encryptedData.saveToFile("encrypted.txt");
        std::cout << "Encrypted data saved to encrypted.txt.\n";

        // Decrypt the data and save the output
        encryptedData.decrypt();  // Renamed from decrypt
        encryptedData.saveToFile("output.txt");
        std::cout << "Decrypted data saved to output.txt.\n";

        // Save the cipher key
        encryptedData.saveKeyToFile("cipher.txt");
        std::cout << "Cipher key saved to cipher.txt.\n";
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
