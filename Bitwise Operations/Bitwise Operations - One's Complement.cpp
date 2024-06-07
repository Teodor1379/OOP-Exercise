#include <bitset>

#include <iostream>

// std::bitset<NUMBER_OF_BITS> name(number);
// std::bitset<NUMBER_OF_BITS> name(string);

int main() {
    int a = 2;
    int b = ~a;

    std::bitset<8> aBits(a);
    std::bitset<8> bBits(b);

    std::cout << aBits << std::endl;
    std::cout << bBits << std::endl;

    return 0;
}
