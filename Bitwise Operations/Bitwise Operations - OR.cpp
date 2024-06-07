#include <iomanip>
#include <iostream>

const int BORDER = 8;

void printBinaryNumber(int number) {
    int copyNumber = number;

    bool digits[BORDER] = { 0 };

    int counter = 0;

    while (number > 0) {
        digits[(BORDER - 1) - counter] = number % 2;
        number = number / 2;
        counter = counter + 1;
    }

    std::cout << std::setw(4) << copyNumber << ": ";

    for (int i = 0; i < BORDER; ++i) {
        std::cout << digits[i];
    }

    std::cout << std::endl;
}

int main() {
    int a = 5;
    int b = 12;
    int c = a | b;

    printBinaryNumber(a);
    printBinaryNumber(b);
    printBinaryNumber(c);
    
    return 0;
}
