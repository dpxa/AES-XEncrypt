std::string get_string(std::ifstream& fin, std::string t) {
    if (!fin)
        return "";
    
    std::ostringstream oss;
    oss << fin.rdbuf(); // Read the entire file buffer into the ostringstream
    return oss.str();
}

std::string get_text(std::string in_file) {
    std::ifstream fin(in_file);
    std::string text;
    return get_string(fin, text);
}

Key get_key(std::string in_file) {
    std::ifstream fin(in_file);
    Key key;
    fin.seekg(5);
    key.cipher = get_string(fin, key.cipher);
    key.start_pos = std::stoi(key.cipher.substr(key.cipher.size()-2));
    key.cipher.erase(key.cipher.size()-2);
    return key;
}