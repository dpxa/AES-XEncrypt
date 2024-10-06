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

    bool isEncrypted = false;

    // characters that can be in the key
    const std::string characterSet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!@#$%^&*()";
    const int keyCreationSize = 20;

    // getTextFromFile and getKeyFromFile both call readFileToString
    std::string readFileToString(std::ifstream& fin);
    void getTextFromFile(const std::string& inputFile);
    void getKeyFromFile(const std::string& inputFile);

public:
    EncryptedText() = default;

    // setText called during each file reached in dfs
    void setText(const std::string& filePath);

    // either key and state are given or not
    void setKeyAndState(const std::string& keyFile, bool encrypt);
    void generateKey();

    bool encrypted() { return isEncrypted; }

    void encrypt();
    void decrypt();

    void saveTextToFile(const std::string& outputFile) const;
    void saveKeyToFile(const std::string& outputFile) const ;
};

#endif // ENCRYPTED_H
