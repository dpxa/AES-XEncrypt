#ifndef ENCRYPTED_H
#define ENCRYPTED_H
#include <string>

// key has a cipher and an arbitrary starting position (not necessary)
struct EncryptionKey {
    std::string cipher;
    int startPosition = 0;
};

class EncryptedText {
private:
    // text to encrypt
    std::string text;
    EncryptionKey key;

    bool encrypted = false;

    // characters that can be in the key
    const std::string characterSet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!@#$%^&*()";
    const int keyCreationSize = 20;

public:
    EncryptedText() = default;

    // setText called during each file reached in dfs
    void setText(const std::string& filePath);
    // for validateKeyFile, it is set if it is valid (we need to check contents, not just file existance)
    bool validateKeyFile(const std::string& inputFile);
    void setState(bool encrypt);

    void generateKey();

    // decide whether to encrypt or decrypt text based on class member encrypted
    void process();

    void encrypt();
    void decrypt();

    void saveTextToFile(const std::string& outputFile) const;
    void saveKeyToFile(const std::string& outputFile) const ;
};

#endif // ENCRYPTED_H
