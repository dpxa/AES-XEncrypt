#include "main.h"

struct Key {
    std::string cipher;
    int start_pos = 0;
};

class Encrypted {
    private:
    std::string text;
    Key key;
    bool encrypt_state = 0;

    public:
    Encrypted() {}
    
    Encrypted(std::string t, bool enc) { text = t; encrypt_state = enc; shuffle(); }

    void set_str(std::string t, bool enc) { text = t; encrypt_state = enc; shuffle(); }
    
    Encrypted(Key k) { key = k; }
    
    void set_cipher(Key k) { key = k; }
    
    Encrypted(std::string t, Key k, bool enc) { text = t; key = k; encrypt_state = enc; }
    
    void set_all(std::string t, Key k, bool enc) { text = t; key = k; encrypt_state = enc; }

    void encrypt();

    void decrypt();

    void shuffle();

    void print_output(std::string out_file) {
        std::ofstream fout(out_file);
        fout << text;
    }

    void print_output_key(std::string out_file) {
        std::ofstream fout(out_file);
        fout << "Key:\n" << key.cipher << key.start_pos;
    }
};