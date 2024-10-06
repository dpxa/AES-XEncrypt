#include "../header/Encrypted.h"

void EncryptedText::setText(const std::string& filePath) {
    getTextFromFile(filePath);
}

void EncryptedText::setKeyAndState(const std::string& keyFile, bool encrypt) {
    getKeyFromFile(keyFile);
    isEncrypted = encrypt;
}