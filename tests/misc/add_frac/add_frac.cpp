#include <iostream>
#include <sstream>
#include <string>
#include <numeric> // For std::gcd

// Function to parse a fraction from a string and return its numerator and denominator
void parseFraction(const std::string& fraction, int& numerator, int& denominator) {
    std::stringstream ss(fraction);
    char slash;
    ss >> numerator >> slash >> denominator;
}

int main(int argc, char* argv[]) {
    // Ensure there are exactly 3 arguments (program name and two fractions)
    if (argc == 3) {
        std::string fraction1 = argv[1];
        std::string fraction2 = argv[2];

        int num1, den1, num2, den2;

        // Parse both fractions
        parseFraction(fraction1, num1, den1);
        parseFraction(fraction2, num2, den2);

        // Compute the sum of the fractions
        int commonDenominator = den1 * den2;
        int sumNumerator = (num1 * den2) + (num2 * den1);

        // Simplify the resulting fraction using std::gcd
        int divisor = std::gcd(std::abs(sumNumerator), std::abs(commonDenominator));
        sumNumerator /= divisor;
        commonDenominator /= divisor;

        // Ensure the denominator is positive
        if (commonDenominator < 0) {
            commonDenominator = -commonDenominator;
            sumNumerator = -sumNumerator;
        }

        if (commonDenominator == 1) {
            // If the denominator is 1, output as an integer
            std::cout << "Sum of " << fraction1 << " and " << fraction2 << " is: "
                      << sumNumerator << std::endl;
        } else {
            std::cout << "Sum of " << fraction1 << " and " << fraction2 << " is: "
                      << sumNumerator << "/" << commonDenominator << std::endl;
        }
    } else {
        std::cout << "Usage: ./program <fraction1> <fraction2>" << std::endl;
    }

    return 0;
}
