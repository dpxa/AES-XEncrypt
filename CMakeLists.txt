cmake_minimum_required(VERSION 3.10)

project(MAC_ENCRYPTION LANGUAGES CXX)

# c++ 17 for filesystem>
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the executable target with main.cpp as the source file
add_executable(mac_encryption 
    src/main.cpp 
    src/Encrypted.cpp 
    src/EncryptedIO.cpp 
    src/EncryptedOp.cpp
    src/DirectoryDFS.cpp
)
