#include "../header/Encrypted.h"

std::string EncryptedText::readFileToString(std::ifstream& fin) {
    std::ostringstream oss;
    oss << fin.rdbuf();
    return oss.str();
}

void EncryptedText::getTextFromFile(const std::string& inputFile) {
    std::ifstream fin(inputFile);
    if (!fin)
        throw std::runtime_error("Failed to open file.");

    text = readFileToString(fin);
}

void EncryptedText::getKeyFromFile(const std::string& inputFile) {
    std::ifstream fin(inputFile);
    if (!fin)
        throw std::runtime_error("Failed to open key file.");

    // skip ahead past "Key: " (this is why we need to pass ifstream and not file name to readFileToString)
    fin.seekg(5);
    // get key from file
    key.cipher = readFileToString(fin);

    // start position is last three characters in the file
    key.startPosition = std::stoi(key.cipher.substr(key.cipher.size() - 3));
    key.cipher.erase(key.cipher.size() - 3);
}

void EncryptedText::saveToFile(const std::string& outputFile) const {
    std::ofstream fout(outputFile);
    if (!fout)
        throw std::runtime_error("Failed to open file for writing encrypted/decrypted text.");
        
    fout << text;
}

void EncryptedText::saveKeyToFile(const std::string& outputFile) const {
    std::ofstream fout(outputFile);
    if (!fout)
        throw std::runtime_error("Failed to open file for writing cipher.");

    fout << "Key:\n" << key.cipher << key.startPosition;
}