#include <iostream>
#include <random>
#include <string>
#include <limits>
#include <iomanip>

constexpr int asciiStart = 33;
constexpr int asciiEnd = 126;
constexpr int maxPasswordLength = 128;

// immediately invoked lambda expression - return string containing each printable ascii character
const std::string charMap = [](){
    std::string map;
    for (char c = asciiStart; c <= asciiEnd; ++c) {
        map += c;
    }
    return map;
}();

std::string generatePassword(int length) {
    /*
    std::random_device: Provides a non-deterministic seed.
    std::mt19937: Generates pseudo-random numbers using the seed from std::random_device.
    std::uniform_int_distribution<>: Ensures the random numbers are uniformly distributed within a specified range.
    */
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, charMap.size() - 1);

    // reserve to optimize memory allocation
    std::string password;
    password.reserve(length);

    for (int i = 0; i < length; ++i) {
        // appends random ascii character
        password += charMap[dis(gen)];
    }

    return password;
}

int getPasswordLength() {
    int passwordSize;
    while (true) {
        std::cout << "Enter password length (0 to exit, max " << maxPasswordLength << "): ";
        if (!(std::cin >> passwordSize) || passwordSize < 0 || passwordSize > maxPasswordLength) {
            // clear input flags
            std::cin.clear();
            // ignore revious input
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a non-negative integer within the allowed range.\n";
        } else {
            return passwordSize;
        }
    }
}

int getNumberOfPasswords() {
    int numberOfPasswords;
    while (true) {
        std::cout << "Enter the number of passwords to generate: ";
        if (!(std::cin >> numberOfPasswords) || numberOfPasswords <= 0) {
            // clear input flags
            std::cin.clear();
            // ignore previous input
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignore invalid input
            std::cout << "Invalid input. Please enter a positive integer.\n";
        } else {
            return numberOfPasswords;
        }
    }
}

void printSeparator() {
    std::cout << "=============================================\n";
}

int main() {
    std::cout << "=============================================\n";
    std::cout << " Welcome to the Random Password Generator! \n";
    std::cout << "=============================================\n";

    while (true) {
        int passwordSize = getPasswordLength();
        if (passwordSize == 0) {
            std::cout << "Exiting program. Thank you for using the Random Password Generator!\n";
            break;
        }

        int numberOfPasswords = getNumberOfPasswords();

        printSeparator();
        for (int i = 0; i < numberOfPasswords; ++i) {
            const std::string password = generatePassword(passwordSize);
            std::cout << "Generated Password " << i + 1 << ":\n";
            std::cout << std::setw(40) << std::left << password << std::endl;
        }
        printSeparator();
        std::cout << "\n";
    }

    return 0;
}
