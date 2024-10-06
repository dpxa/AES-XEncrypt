#include "../header/Encrypted.h"

EncryptedText::EncryptedText(const std::string& filePath, bool encrypt) : isEncrypted(encrypt) {
    getTextFromFile(filePath);

    if (!hasCustomKey)
        generateKey();
}

EncryptedText::EncryptedText(const std::string& keyFile) : hasCustomKey(true) { 
    getKeyFromFile(keyFile);
}

EncryptedText::EncryptedText(const std::string& filePath, const std::string& keyFile, bool encrypt) : hasCustomKey(true), isEncrypted(encrypt) {
    getTextFromFile(filePath);
    getKeyFromFile(keyFile);
}

void EncryptedText::setTextAndState(const std::string& filePath, bool encrypt) {
    getTextFromFile(filePath);
    isEncrypted = encrypt;

    if (!hasCustomKey)
        generateKey();
}

void EncryptedText::setKeyFromFile(const std::string& keyFile) {
    getKeyFromFile(keyFile);
    hasCustomKey = true;
}

// setter for text, key, and encryption state
void EncryptedText::setAll(const std::string& filePath, const std::string& keyFile, bool encrypt) {
    getTextFromFile(filePath);
    getKeyFromFile(keyFile);
    hasCustomKey = true;
    isEncrypted = encrypt;
}