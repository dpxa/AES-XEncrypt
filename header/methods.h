#ifndef METHODS_H
#define METHODS_H

#include "main.h"
#include "Encrypted.h"

// read entire content of a file to a string
std::string get_string(std::ifstream& fin) {
    if (!fin) {
        throw std::runtime_error("Failed to open file.");
    }

    std::ostringstream oss;
    oss << fin.rdbuf();
    return oss.str();
}

// get the text from a file
std::string get_text(const std::string& in_file) {
    std::ifstream fin(in_file);
    return get_string(fin);
}

// get the key from a file
Key get_key(const std::string& in_file) {
    std::ifstream fin(in_file);
    if (!fin)
        throw std::runtime_error("Failed to open file.");

    Key key;
    fin.seekg(5);
    key.cipher = get_string(fin);
    
    if (key.cipher.size() < 2)
        throw std::runtime_error("Invalid key format.");

    key.start_pos = std::stoi(key.cipher.substr(key.cipher.size() - 2));
    key.cipher.erase(key.cipher.size() - 2);
    return key;
}

#endif // METHODS_H
