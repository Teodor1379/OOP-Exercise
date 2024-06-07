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

    std::cout << std::setw(4) << copyNumber << ": ";

    for (int i = 0; i < BORDER; ++i) {
        if (i == 4) {
            std::cout << ' ';
        }

        std::cout << digits[i];
    }

    std::cout << std::endl;
}

int main() {
    int a = 2;
    int b = a >> 1;

    // int a = -2;
    // int b = a >> 1; // the value of b depends on the compiler

    // int a = 1;
    // int b = a >> -1; // undefined behaviour

    printBinaryNumber(a);
    printBinaryNumber(b);

    return 0;
}
