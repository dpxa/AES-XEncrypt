#include "../header/Encrypted.h"
#include "../header/AesObject.h"
#include <random>

void EncryptedText::encrypt() {
    AesObject aes;

    // padding - data size must be multiple of 16
    aes.Pad(data);

    uint8_t rks[aes.expKeySize];
    aes.keyEx(key, rks);

    size_t n = data.size();
    for (size_t i = 0; i < n; i += 16) {
        aes.encB(&data[i], rks);
    }
}

void EncryptedText::decrypt() {
    AesObject aes;

    uint8_t rks[aes.expKeySize];
    aes.keyEx(key, rks);

    size_t n = data.size();
    for (size_t i = 0; i < n; i += 16) {
        aes.decB(&data[i], rks);
    }

    aes.Unpad(data);
}

void EncryptedText::generateKey() {
    // high-entropy
    std::random_device rd;

    for (int i = 0; i < AES_KEY_SIZE; ++i) {
        key[i] = static_cast<uint8_t>(rd() % 256);
    }
}
