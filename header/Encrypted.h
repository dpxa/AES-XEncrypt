#ifndef ENCRYPTED_H
#define ENCRYPTED_H
#include <string>
#include <QListWidget>

// key has a cipher and an arbitrary starting position
struct EncryptionKey {
    std::string cipher;
    int startPosition = 0;
};

class EncryptedText {
private:
    std::string text;
    EncryptionKey key;

    std::string keyFile;

    bool encrypted = false;

    static const std::string characterSet;
    static const int keyCreationSize;

    void generateKey();

    void encrypt();
    void decrypt();

public:
    EncryptedText() = default;

    void setText(const std::string& file);

    // for validateKeyFile
    enum class KeyFileStatus {
        FileNotFound,
        InvalidKey,
        ValidKey
    };
    // set key to content of file and return code
    KeyFileStatus validateKeyFile(const std::string& file);

    bool newKey(const std::string& file);

    void setState(bool enc) { encrypted = enc; }
    bool isEncrypted() { return encrypted; }

    void process() { (encrypted) ? decrypt() : encrypt(); }

    void saveTextToFile(const std::string& file) const;

    // on first encrypt of new key
    void saveKeyToFile() const;
};

#endif // ENCRYPTED_H
