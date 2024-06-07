#include <string>

#include <cmath>

#include <iomanip>
#include <iostream>

const int BORDER = 8;

char getLetter(int remainder) {
    if (remainder < 10) {
        return remainder + '0';
    } else {
        switch (remainder) {
            case 10: return 'A';
            case 11: return 'B';
            case 12: return 'C';
            case 13: return 'D';
            case 14: return 'E';
            case 15: return 'F';
            default: return '\0';
        }
    }
}

long long getDigit(char letter) {
    if (letter >= '0' && letter <= '9') {
        return letter - '0';
    } else {
        switch (letter) {
            case 'A': return 10;
            case 'B': return 11;
            case 'C': return 12;
            case 'D': return 13;
            case 'E': return 14;
            case 'F': return 15;
            default: return -1;
        }
    }
}

void printHexademicalNumber(long long number) {
    long long copyNumber = number;

    char digits[BORDER] = { '\0' };

    int counter = 0;

    while (number > 0) {
        char letter = getLetter(number % 16);
        digits[(BORDER - 1) - counter] = letter;
        number = number / 16;
        counter = counter + 1;
    }

    std::cout << std::setw(4) << copyNumber << ": " << "0x";

    for (int i = 0; i < BORDER; ++i) {
        std::cout << digits[i];
    }

    std::cout << std::endl;
}

long long getDecimalNumber(const std::string& number) {
    long long result = 0;
    long long power = 0;

    for (int i = number.size() - 1; i >= 0; --i) {
        long long current = getDigit(number[i]) * std::pow(16, power);
        result = result + current; power = power + 1;
    }

    return result;
}

int main() {
    printHexademicalNumber(1);
    printHexademicalNumber(2);
    printHexademicalNumber(3);

    printHexademicalNumber(100);
    printHexademicalNumber(200);
    printHexademicalNumber(300);

    printHexademicalNumber(1000);
    printHexademicalNumber(2000);
    printHexademicalNumber(3000);

    std::cout << getDecimalNumber("1") << std::endl;
    std::cout << getDecimalNumber("2") << std::endl;
    std::cout << getDecimalNumber("3") << std::endl;
    std::cout << getDecimalNumber("64") << std::endl;
    std::cout << getDecimalNumber("C8") << std::endl;
    std::cout << getDecimalNumber("12C") << std::endl;
    std::cout << getDecimalNumber("3E8") << std::endl;
    std::cout << getDecimalNumber("7D0") << std::endl;
    std::cout << getDecimalNumber("BB8") << std::endl;

    return 0;
}
