#include <string>

#include <cmath>

#include <iomanip>
#include <iostream>

const int BORDER = 8;

void printOctalNumber(long long number) {
    long long copyNumber = number;

    int digits[BORDER] = { 0 };

    int counter = 0;

    while (number > 0) {
        digits[(BORDER - 1) - counter] = number % 8;
        number = number / 8;
        counter = counter + 1;
    }

    std::cout << std::setw(4) << copyNumber << ": " << "0o";

    for (int i = 0; i < BORDER; ++i) {
        std::cout << digits[i];
    }

    std::cout << std::endl;
}

long long getDecimalNumber(const std::string& number) {
    long long result = 0;
    long long power = 0;

    for (int i = number.size() - 1; i >= 0; --i) {
        long long current = (number[i] - '0') * std::pow(8, power);
        result = result + current; power = power + 1;
    }

    return result;
}

int main() {
    printOctalNumber(1);
    printOctalNumber(2);
    printOctalNumber(3);

    printOctalNumber(100);
    printOctalNumber(200);
    printOctalNumber(300);

    printOctalNumber(1000);
    printOctalNumber(2000);
    printOctalNumber(3000);

    std::cout << getDecimalNumber("00000001") << std::endl;
    std::cout << getDecimalNumber("00000002") << std::endl;
    std::cout << getDecimalNumber("00000003") << std::endl;
    std::cout << getDecimalNumber("00000144") << std::endl;
    std::cout << getDecimalNumber("00000310") << std::endl;
    std::cout << getDecimalNumber("00000454") << std::endl;
    std::cout << getDecimalNumber("00001750") << std::endl;
    std::cout << getDecimalNumber("00003720") << std::endl;
    std::cout << getDecimalNumber("00005670") << std::endl;

    return 0;
}
