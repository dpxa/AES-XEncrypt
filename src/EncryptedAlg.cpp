#include "../header/Encrypted.h"
#include <random>

void EncryptedText::encrypt() {
    size_t text_size = text.size();
    if (text_size == 0)
        return;
    
    size_t cipher_size = key.cipher.size();
    size_t offset = 0;
    size_t pos = key.startPosition % text_size;
    
    for (size_t i = 0; i < text_size; ++i) {
        char& currentChar = text[pos];

        // if character is ascii, encrypt it
        if (static_cast<unsigned char>(currentChar) <= 127) {
            currentChar = (currentChar + key.cipher[offset]) % 128;
        }
        
        if (++offset == cipher_size) offset = 0;
        if (++pos == text_size) pos = 0;
    }
}

void EncryptedText::decrypt() {
    size_t text_size = text.size();
    if (text_size == 0)
        return;
    
    size_t cipher_size = key.cipher.size();
    size_t offset = 0;
    size_t pos = key.startPosition % text_size;
        
    for (size_t i = 0; i < text_size; ++i) {
        char& currentChar = text[pos];
        
        // if character is ascii, decrypt it
        if (static_cast<unsigned char>(currentChar) <= 127) {
            currentChar = (currentChar - key.cipher[offset] + 128) % 128;
        }
        
        if (++offset == cipher_size) offset = 0;
        if (++pos == text_size) pos = 0;
    }
}

void EncryptedText::generateKey() {
    std::random_device rd;
    std::mt19937 rng(rd());

    // uniform distribution over all indexes in characterSet
    std::uniform_int_distribution<int> charDistribution(0, characterSet.size() - 1);
    key.cipher.clear();
    
    // get random characters
    for (int i = 0; i < keyCreationSize; ++i)
        key.cipher += characterSet[charDistribution(rng)];

    // uniform distribution over all three digit numbers (starting position)
    std::uniform_int_distribution<int> posDistribution(100, 999);
    key.startPosition = posDistribution(rng);
}
