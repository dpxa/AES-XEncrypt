# CMake minimum version
cmake_minimum_required(VERSION 3.10)

# Project name and the programming language used
project(MAC_ENCRYPTION LANGUAGES CXX)

# Set the C++ standard (optional, but recommended)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the executable target with main.cpp as the source file
add_executable(mac_encryption src/main.cpp src/Encrypted.cpp)
