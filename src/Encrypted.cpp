#include "../header/Encrypted.h"

// Encrypt the text using the cipher key
void Encrypted::encrypt() {
    if (!encrypt_state) {
        int offset = 0;
        int pos = key.start_pos;
    
        for (size_t i = 0; i < text.size(); ++i) {
            char& current_char = text[pos];
            current_char = (current_char + key.cipher[offset]) % 128;
            
            offset = (offset + 1) % 20;
            pos = (pos + 1) % text.size();
        }
        
        encrypt_state = true;
    }
}

// Decrypt the text using the cipher key
void Encrypted::decrypt() {
    if (encrypt_state) {
        int offset = 0;
        int pos = key.start_pos;
        
        for (size_t i = 0; i < text.size(); ++i) {
            char& current_char = text[pos];
            current_char = (current_char + 128 - key.cipher[offset]) % 128;
            
            offset = (offset + 1) % 20;
            pos = (pos + 1) % text.size();
        }
        
        encrypt_state = false;
    }
}

void Encrypted::createKey() {
    std::random_device rd;
    std::mt19937 rng(rd());

    std::uniform_int_distribution<int> distribution(0, character_set.size() - 1);
    key.cipher.clear();
    
    for (int i = 0; i < 20; ++i)
        key.cipher += character_set[distribution(rng)];

    key.start_pos = distribution(rng);
}