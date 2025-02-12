#include "../header/Encrypted.h"
#include <fstream>
#include <filesystem>

void EncryptedText::setData(const std::string& inputFile) {
    std::ifstream fin(inputFile, std::ios::binary);
    if (!fin) {
        throw std::runtime_error("Failed to open file for reading text.");
    }

    // get size and resize data vector
    fin.seekg(0, std::ios::end);
    std::streamsize fileSize = fin.tellg();
    fin.seekg(0, std::ios::beg);

    data.resize(fileSize);

    fin.read(reinterpret_cast<char*>(data.data()), fileSize);
    fin.close();
}

EncryptedText::KeyFileStatus EncryptedText::validateKeyFile(const std::string &inputFile)
{
    std::ifstream fin(inputFile, std::ios::binary);
    if (!fin)
    {
        fin.close();
        return KeyFileStatus::FileNotFound;
    }

    std::string line;
    std::getline(fin, line);

    // should have "Key:" t the beginning if key was created by this program
    if (line != "Key:") {
        fin.close();
        return KeyFileStatus::InvalidKey;
    }

    fin.read(reinterpret_cast<char *>(key), AES_KEY_SIZE);

    // AES_KEY_SIZE chars exactly
    if (fin.gcount() != AES_KEY_SIZE)
    {
        fin.close();
        return KeyFileStatus::InvalidKey;
    }

    char extra;
    if (fin.get(extra))
    {
        fin.close();
        return KeyFileStatus::InvalidKey;
    }

    fin.close();
    return KeyFileStatus::ValidKey;
}


bool EncryptedText::newKey(const std::string& file) {
    std::filesystem::path filePath(file);

    // if the file is already a directory
    if (std::filesystem::is_directory(filePath)) {
        return false;
    }

    // if the parent directory does not exist
    if (!std::filesystem::is_directory(filePath.parent_path())) {
        return false;
    }

    generateKey();
    keyFile = file;
    return true;
}

void EncryptedText::saveData(const std::string& outputFile) const {
    std::ofstream fout(outputFile, std::ios::binary);
    if (!fout)
        throw std::runtime_error("Failed to open file for writing encrypted/decrypted text.");

    fout.write(reinterpret_cast<const char*>(data.data()), data.size());
    fout.close();
}

void EncryptedText::saveKey() const {
    std::ofstream fout(keyFile, std::ios::binary);
    if (!fout)
        throw std::runtime_error("Failed to open file for writing encrypted/decrypted text.");

    fout << "Key:\n";
    fout.write(reinterpret_cast<const char*>(key), AES_KEY_SIZE);
    fout.close();
}
