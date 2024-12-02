#include "../header/Encrypted.h"
#include <fstream>
#include <sstream>
#include <filesystem>

const std::string EncryptedText::characterSet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!@#$%^&*()";
const int EncryptedText::keyCreationSize = 20;

void EncryptedText::setText(const std::string& inputFile) {
    // open file in binary mode, as characters will be ascii 0-127
    std::ifstream fin(inputFile, std::ios::binary);
    if (!fin)
        // should never be thrown - root file validated in ddfs
        throw std::runtime_error("Failed to open file for reading text.");
    
    std::ostringstream oss;
    oss << fin.rdbuf();
    text = oss.str();
}

EncryptedText::KeyFileStatus EncryptedText::validateKeyFile(const std::string& inputFile) {
    std::ifstream fin(inputFile);
    if (!fin) {
        return KeyFileStatus::FileNotFound;
    }

    std::string line;
    std::getline(fin, line);
    if (line != "Key:") {
        return KeyFileStatus::InvalidKey;
    }

    std::string k;
    char ch;
    for (int i = 0; i < 23; ++i) {
        if (fin.get(ch)) {
            k += ch;
            if (i >= 20 && !std::isdigit(ch)) {
                return KeyFileStatus::InvalidKey;
            }
        } else {
            return KeyFileStatus::InvalidKey;
        }
    }

    char extra;
    if (fin.get(extra)) {
        return KeyFileStatus::InvalidKey;
    }

    key.cipher = k.substr(0, 20);
    key.startPosition = std::stoi(k.substr(20, 3));

    return KeyFileStatus::ValidKey;
}

bool EncryptedText::newKey(const std::string& file) {
    std::filesystem::path filePath(file);
    // check if the file can exist
    if (std::filesystem::is_directory(filePath)) {
        return false;
    }

    if (!std::filesystem::is_directory(filePath.parent_path())) {
        return false;
    }

    generateKey();
    keyFile = file;
    return true;
}

void EncryptedText::saveTextToFile(const std::string& outputFile) const {
    std::ofstream fout(outputFile, std::ios::binary);
    if (!fout)
        // should never be thrown
        throw std::runtime_error("Failed to open file for writing encrypted/decrypted text.");
        
    fout << text;
}

void EncryptedText::saveKeyToFile() const {
    std::ofstream fout(keyFile);
    if (!fout)
        // should never be thrown
        throw std::runtime_error("Failed to open file for writing encrypted/decrypted text.");

    fout << "Key:\n" << key.cipher << key.startPosition;
}
