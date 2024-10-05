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

// Shuffle the text and generate a random start position for encryption
void Encrypted::shuffle() {
    std::random_device rd;
    std::mt19937 rng(rd());
    
    key.cipher = text; // Copy text to cipher key
    std::shuffle(key.cipher.begin(), key.cipher.end(), rng); // Shuffle cipher key
    key.cipher = key.cipher.substr(0, 20); // Keep only the first 20 characters

    std::uniform_int_distribution<int> distribution(0, text.size() - 1);
    key.start_pos = distribution(rng); // Randomly select start position
}
