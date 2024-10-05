#include "../header/Encrypted.h"
#include "../header/Credit.h"
#include "../header/methods.h"

int main() {
    try {
        // Create Date objects for expiration dates
        Date expirationDate1(12, 30);
        Date expirationDate2(1, 15);
        Date expirationDate3(6, 25);

        // Create Credit objects for three different cards
        Credit creditCard1("1234 1234 1234 1234", 789, expirationDate1);
        Credit creditCard2("4321 4321 4321 4321", 456, expirationDate2);
        Credit creditCard3("5678 5678 5678 5678", 123, expirationDate3);

        // Write credit card information to the file
        creditCard1.save("info.txt");
        creditCard2.save("info.txt");
        creditCard3.save("info.txt");
        std::cout << "All credit card information saved to info.txt.\n";

        // Read text from the file and create an Encrypted object
        Encrypted encryptedData(get_text("info.txt"), false);
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
