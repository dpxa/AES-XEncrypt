#include "Encrypted.h"

// when text is mutated in an Encrypted object, it is copied to the cipher key, shuffled, 
// and the first 20 chracters act as the cipher key
// then where the cipher key starts encrypting text is randomly generated and
// stored as the cipher start_pos

void Encrypted::encrypt() {
    if (!encrypt_state) {
        int offset = 0;
        int pos = key.start_pos;
    
        for (int i = 0; i < text.size(); i++) {
            char& current = text[pos];
            current = (current + key.cipher[offset]) % 128;
            
            offset = (offset + 1) % 20;
            pos = (pos + 1) % text.size();
        }
            
        encrypt_state = 1;
    }
}

void Encrypted::decrypt() {
    if (encrypt_state) {
        int offset = 0;
        int pos = key.start_pos;
            
        for (int i = 0; i < text.size(); i++) {
            char& current = text[pos];
            current = (current + 128 - key.cipher[offset]) % 128;
            
            offset = (offset + 1) % 20;
            pos = (pos + 1) % text.size();
        }
            
        encrypt_state = 0;
    }
}

void Encrypted::shuffle() {
    std::random_device rd;
    std::mt19937 rng(rd());
    key.cipher = text;
    std::shuffle(key.cipher.begin(), key.cipher.end(), rng);
    key.cipher = key.cipher.substr(0, 20);

    std::uniform_int_distribution<int> uni(0, text.size() - 1);
    key.start_pos = uni(rng);
}