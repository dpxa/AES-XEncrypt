#ifndef ENCRYPTED_H
#define ENCRYPTED_H
#include <string>

struct EncryptionKey {
    std::string cipher;
    int startPosition = 0;
};

class EncryptedText {
private:
    std::string text;
    EncryptionKey key;
    bool isEncrypted = false;
    bool hasCustomKey = false;
    const std::string characterSet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!@#$%^&*()";
    const int keyCreationSize = 20;

    std::string readFileToString(std::ifstream& fin);
    void getTextFromFile(const std::string& inputFile);
    void getKeyFromFile(const std::string& inputFile);

public:
    EncryptedText() = default;
    EncryptedText(const std::string& filePath, bool encrypt);
    EncryptedText(const std::string& keyFile);
    EncryptedText(const std::string& filePath, const std::string& keyFile, bool encrypt);

    void setTextAndState(const std::string& filePath, bool encrypt);
    void setKeyFromFile(const std::string& keyFile);
    void setAll(const std::string& filePath, const std::string& keyFile, bool encrypt);

    void generateKey();
    void encrypt();
    void decrypt();

    void saveToFile(const std::string& outputFile) const;
    void saveKeyToFile(const std::string& outputFile) const ;
};

#endif // ENCRYPTED_H
