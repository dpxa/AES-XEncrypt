#include "../header/Encrypted.h"
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <vector>
#include <memory>

void EncryptedText::encrypt() {
    auto ctx = std::unique_ptr<EVP_CIPHER_CTX, decltype(&EVP_CIPHER_CTX_free)>(
        EVP_CIPHER_CTX_new(), EVP_CIPHER_CTX_free
    );
    if (!ctx) return;

    if (!EVP_EncryptInit_ex(ctx.get(), EVP_aes_256_ecb(), nullptr, key, nullptr)) return;

    std::vector<uint8_t> ciphertext(data.size() + EVP_CIPHER_block_size(EVP_aes_256_ecb()));
    int len;
    int ciphertext_len;
    
    // does not pad data
    if (!EVP_EncryptUpdate(ctx.get(), ciphertext.data(), &len, data.data(), data.size())) return;
    ciphertext_len = len;

    if (!EVP_EncryptFinal_ex(ctx.get(), ciphertext.data() + len, &len)) return;
    ciphertext_len += len;
    
    ciphertext.resize(ciphertext_len);
    data = std::move(ciphertext);
}

void EncryptedText::decrypt() {
    auto ctx = std::unique_ptr<EVP_CIPHER_CTX, decltype(&EVP_CIPHER_CTX_free)>(
        EVP_CIPHER_CTX_new(), EVP_CIPHER_CTX_free
    );
    if (!ctx) return;

    if (!EVP_DecryptInit_ex(ctx.get(), EVP_aes_256_ecb(), nullptr, key, nullptr)) return;

    std::vector<uint8_t> plaintext(data.size());
    int len;
    int plaintext_len;
    
    // does not unpad data
    if (!EVP_DecryptUpdate(ctx.get(), plaintext.data(), &len, data.data(), data.size())) return;
    plaintext_len = len;

    if (!EVP_DecryptFinal_ex(ctx.get(), plaintext.data() + len, &len)) return;
    plaintext_len += len;

    plaintext.resize(plaintext_len);
    data = std::move(plaintext);
}

void EncryptedText::generateKey() {
    RAND_bytes(key, AES_KEY_SIZE);
}
