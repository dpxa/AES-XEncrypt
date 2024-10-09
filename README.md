v-directory-encryptor is a command-line tool designed to encrypt or decrypt a file or all files within a specified directory and its subdirectories using a custom Vigenère cipher. This project provides a secure method to protect sensitive data stored in directories.

Does not encrypt non ascii characters (skips over them).

Encryption algorithm not safe to use.

Requirements:
  CMake

Optional:
  Powershell

To Compile
  with powershell:
    .\build.ps1
  without:
    cmake -S . -B build
    cmake --build build

To run
  with powershell:
    .\dir-enc.exe
  without:
    .\build\dir-enc.exe
