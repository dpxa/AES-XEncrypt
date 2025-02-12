#ifndef ENCRYPTED_H
#define ENCRYPTED_H

#include <string>
#include <vector>
#include <cstdint>

static constexpr int AES_KEY_SIZE = 32;

class EncryptedText {
private:
    std::vector<uint8_t> data;
    uint8_t key[AES_KEY_SIZE] = {0};

    std::string keyFile;
    bool encrypted = false;

    void generateKey();
    void encrypt();
    void decrypt();

public:
    EncryptedText() = default;

    // recursively called in ddfs
    void setData(const std::string& file);
    void saveData(const std::string& file) const;

    enum class KeyFileStatus {
        FileNotFound,
        InvalidKey,
        ValidKey
    };
    KeyFileStatus validateKeyFile(const std::string& file);
    bool newKey(const std::string& file);

    void setState(bool enc) { encrypted = enc; }
    bool isEncrypted() const { return encrypted; }

    void process() { (encrypted) ? decrypt() : encrypt(); }

    // only used if key file did not exist before
    void saveKey() const;
};

#endif // ENCRYPTED_H
