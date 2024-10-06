#include "../header/Encrypted.h"
#include <fstream>
#include <sstream>

std::string EncryptedText::readFileToString(std::ifstream& fin) {
    // create output string stream
    std::ostringstream oss;
    // read the entire file buffer into the string stream
    oss << fin.rdbuf();
    // convert string stream to std::string
    return oss.str();
}

void EncryptedText::getTextFromFile(const std::string& inputFile) {
    // open file in binary mode, as characters will be ascii 0-127
    std::ifstream fin(inputFile, std::ios::binary);
    if (!fin)
        throw std::runtime_error("Failed to open file.");

    text = readFileToString(fin);
}

void EncryptedText::getKeyFromFile(const std::string& inputFile) {
    std::ifstream fin(inputFile);
    if (!fin)
        throw std::runtime_error("Failed to open key file.");

    // skip ahead past "Key: " (this is why we need to pass ifstream and not file name to readFileToString)
    // skip 6 characters "Key:\r\n" on windows
    fin.seekg(6);
    // get key from file, including the starting position
    key.cipher = readFileToString(fin);

    // extract starting position from cipher
    key.startPosition = std::stoi(key.cipher.substr(key.cipher.size() - 3));
    // remove starting position from cipher
    key.cipher.erase(key.cipher.size() - 3);
}

void EncryptedText::saveTextToFile(const std::string& outputFile) const {
    // if file cannot be opened in binary and is some other format (called during dfs)
    std::ofstream fout(outputFile, std::ios::binary);
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