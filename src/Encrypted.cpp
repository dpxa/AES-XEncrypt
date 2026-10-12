#include "../header/Encrypted.h"
#include <fstream>
#include <filesystem>

bool EncryptedText::setData(const std::string& inputFile) {
    std::ifstream fin(inputFile, std::ios::binary);
    if (!fin) {
        return false;
    }

    // get size and resize data vector
    fin.seekg(0, std::ios::end);
    std::streamsize fileSize = fin.tellg();
    if (fileSize < 0) {
        return false;
    }
    fin.seekg(0, std::ios::beg);

    data.resize(fileSize);

    fin.read(reinterpret_cast<char*>(data.data()), fileSize);
    return fin.good() || fin.gcount() == fileSize;
}

EncryptedText::KeyFileStatus EncryptedText::validateKeyFile(const std::string &inputFile)
{
    std::ifstream fin(inputFile, std::ios::binary);
    if (!fin)
    {
        return KeyFileStatus::FileNotFound;
    }

    std::string line;
    std::getline(fin, line);

    // should have "Key:" at the beginning if key was created by this program
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

bool EncryptedText::saveData(const std::string& outputFile) const {
    const std::filesystem::path outputPath(outputFile);
    std::filesystem::path tempPath = outputPath;
    tempPath += ".tmp";
    std::filesystem::path backupPath = outputPath;
    backupPath += ".bak";

    std::error_code error;
    std::filesystem::remove(tempPath, error);
    std::filesystem::remove(backupPath, error);

    std::ofstream fout(tempPath, std::ios::binary | std::ios::trunc);
    if (!fout) {
        return false;
    }

    fout.write(reinterpret_cast<const char*>(data.data()), data.size());
    fout.close();
    if (!fout.good()) {
        std::filesystem::remove(tempPath, error);
        return false;
    }

    std::filesystem::rename(outputPath, backupPath, error);
    if (error) {
        std::filesystem::remove(tempPath, error);
        return false;
    }

    std::filesystem::rename(tempPath, outputPath, error);
    if (error) {
        std::filesystem::remove(tempPath, error);
        std::filesystem::rename(backupPath, outputPath, error);
        return false;
    }

    std::filesystem::remove(backupPath, error);
    return true;
}

void EncryptedText::saveKey() const {
    std::ofstream fout(keyFile, std::ios::binary);
    if (!fout)
        throw std::runtime_error("Failed to open file for writing key.");

    fout << "Key:\n";
    fout.write(reinterpret_cast<const char*>(key), AES_KEY_SIZE);
    fout.close();
}
