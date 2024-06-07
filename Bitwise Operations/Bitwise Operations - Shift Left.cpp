#include <bitset>

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

    std::cout << std::setw(2) << copyNumber << ": ";

    for (int i = 0; i < BORDER; ++i) {
        if (i == 4) {
            std::cout << ' ';
        }

        std::cout << digits[i];
    }

    std::cout << std::endl;
}

long long fastExponent(int power) {
    // Only works for 2!
    
    // return 1 << power;
    return 2 << (power - 1);
}

int main() {
    int a = 1;
    int b = a << 1;

    // int a = 1;
    // int b = a << -1; // undefined behaviour
    // char c = a << 20; // undefined behaviour

    printBinaryNumber(a);
    printBinaryNumber(b);

    std::cout << fastExponent(2) << std::endl;
    std::cout << fastExponent(3) << std::endl;
    std::cout << fastExponent(4) << std::endl;

    return 0;
}
