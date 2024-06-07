#include <string>

#include <cmath>

#include <iomanip>
#include <iostream>

const int BORDER = 8;

void printBinaryNumber(long long number) {
    long long copyNumber = number;

    bool digits[BORDER] = { 0 };
    
    int counter = 0;

    while (number > 0) {
        digits[(BORDER - 1) - counter] = number % 2;
        number = number / 2;
        counter = counter + 1;
    }

    std::cout << std::setw(4) << copyNumber << ": " << "0b";

    for (int i = 0; i < BORDER; ++i) {
        if (i == 4) {
            std::cout << ' ';
        }

        std::cout << digits[i];
    }

    std::cout << std::endl;
}

long long getDecimalNumber(const std::string& number) {
    long long result = 0;
    long long power = 0;

    for (int i = number.size() - 1; i >= 0; --i) {
        long long current = (number[i] - '0') * std::pow(2, power);
        result = result + current; power = power + 1;
    }

    return result;
}

int main() {
    printBinaryNumber(1);
    printBinaryNumber(2);
    printBinaryNumber(3);
    printBinaryNumber(4);
    printBinaryNumber(5);
    printBinaryNumber(6);
    printBinaryNumber(7);
    printBinaryNumber(8);
    printBinaryNumber(9);

    std::cout << getDecimalNumber("00000001") << std::endl;
    std::cout << getDecimalNumber("00000010") << std::endl;
    std::cout << getDecimalNumber("00000011") << std::endl;
    std::cout << getDecimalNumber("00000100") << std::endl;
    std::cout << getDecimalNumber("00000101") << std::endl;
    std::cout << getDecimalNumber("00000110") << std::endl;
    std::cout << getDecimalNumber("00000111") << std::endl;
    std::cout << getDecimalNumber("00001000") << std::endl;
    std::cout << getDecimalNumber("00001001") << std::endl;

    return 0;
}
