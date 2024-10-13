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
        // ideally should never be thrown, as we already checked if this is a regular file
        throw std::runtime_error("Failed to open file for reading text.");
    
    // create output string stream
    std::ostringstream oss;
    // read the entire file buffer into the string stream
    oss << fin.rdbuf();
    // convert string stream to std::string
    text = oss.str();
}

bool EncryptedText::validateKeyFile(const std::string& inputFile) {
    std::ifstream fin(inputFile);
    if (!fin)
        return false;

    std::string line;
    // Check for "Key:" at the top (getline stops at '\n')
    std::getline(fin, line);
    if (line != "Key:") {
        return false;
    }

    // Check for 20 random characters followed by 3 digits
    std::string k;
    char ch;
    for (int i = 0; i < 23; ++i) {
        if (fin.get(ch)) {
            k += ch;
            if (i >= 20 && !std::isdigit(ch)) {
                return false;
            }
        } else {
            return false;
        }
    }

    // ensure there are no extra characters
    char extra;
    if (fin.get(extra)) {
        return false;
    }

    // everything is correct, so extract key
    key.cipher = k.substr(0, 20);
    key.startPosition = std::stoi(k.substr(20, 3));

    return true;
}

bool EncryptedText::newKey(const std::string& inputFile) {
    std::filesystem::path filePath(inputFile);
    if (std::filesystem::is_directory(filePath)) {
        return false;
    }

    if (!std::filesystem::is_directory(filePath.parent_path()))
        return false;

    generateKey();
    keyFile = inputFile;
    return true;
}

void EncryptedText::setState(bool encrypt) {
    doEncrypt = encrypt;
}

void EncryptedText::saveTextToFile(const std::string& outputFile) const {
    // if file cannot be opened in binary and is some other format (called during dfs)
    std::ofstream fout(outputFile, std::ios::binary);
    if (!fout)
        throw std::runtime_error("Failed to open file for writing encrypted/decrypted text.");
        
    fout << text;
}

void EncryptedText::saveKeyToFile() const {
    std::ofstream fout(keyFile);
    if (!fout)
        // should never be reached
        throw std::runtime_error("Failed to open file for writing encrypted/decrypted text.");

    fout << "Key:\n" << key.cipher << key.startPosition;
}
