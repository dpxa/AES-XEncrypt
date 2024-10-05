#ifndef ENCRYPTED_H
#define ENCRYPTED_H

#include "main.h"

struct Key {
    std::string cipher;
    int start_pos = 0;
};

class Encrypted {
private:
    std::string text;
    Key key;
    bool encrypt_state = false;
    const std::string character_set = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!@#$%^&*()";

public:
    // Default constructor
    Encrypted() = default;

    // Constructor to initialize text and encryption state
    Encrypted(const std::string& t, bool enc) 
        : text(t), encrypt_state(enc) {
        createKey();
    }

    // Constructor to initialize key
    Encrypted(const Key& k) 
        : key(k) {}

    // Constructor to initialize text, key, and encryption state
    Encrypted(const std::string& t, const Key& k, bool enc) 
        : text(t), key(k), encrypt_state(enc) {}

    // Setter for text and encryption state
    void setStr(const std::string& t, bool enc) {
        text = t;
        encrypt_state = enc;
        createKey();
    }

    // Setter for key
    void setCipher(const Key& k) {
        key = k;
    }

    // Setter for all attributes
    void setAll(const std::string& t, const Key& k, bool enc) {
        text = t;
        key = k;
        encrypt_state = enc;
    }

    // Methods for encryption, decryption, and shuffling
    void encrypt();
    void decrypt();
    void createKey();

    // Output the text to a file
    void save(const std::string& out_file) const {
        std::ofstream fout(out_file);
        if (!fout)
            throw std::runtime_error("Failed to open file for writing encrypted/decrypted text.");
        
        fout << text;
    }

    // Output the key to a file
    void saveKey(const std::string& out_file) const {
        std::ofstream fout(out_file);
        if (!fout)
            throw std::runtime_error("Failed to open file for writing cipher.");

        fout << "Key:\n" << key.cipher << key.start_pos;
    }
};

#endif // ENCRYPTED_H