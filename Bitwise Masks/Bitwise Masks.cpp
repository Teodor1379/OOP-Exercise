#include <bitset>

#include <string>

#include <iostream>

bool getBitValue(int number, int index) {
    // Index is 0-based, from right to left.

    // Example: 00001100
    // Bit 0 is: 0
    // Bit 1 is: 0
    // Bit 2 is: 1
    // Bit 3 is: 1

    return 1 & (number >> index);
}

int makeBit1(int number, int index) {
    // SetBit - Makes the bit to 1 on position "index".

    number = number | (1 << index);
    return number;
}

int makeBit0(int number, int index) {
    // ClearBit - Makes the bit to 0 on position "index".
    // Uncomment to see every step of the process.

    /*
        std::bitset<8> bitSet1(0b00001100);
        std::bitset<8> bitSet2(1 << 2);
        std::bitset<8> bitSet3(~(1 << 2));
        std::bitset<8> bitSet4(0b00001100 & ~(1 << 2));

        std::cout << bitSet1 << std::endl;
        std::cout << bitSet2 << std::endl;
        std::cout << bitSet3 << std::endl;
        std::cout << bitSet4 << std::endl;
    */

    number = number & ~(1 << index);
    return number;
}

int makeBit01(int number, int index) {
    // ToggleBit - Toggles the bit from 1 to 0
    // or from 0 to 1 on position "index".

    number = number ^ (1 << index);
    return number;
}

int main() {
    int a = 12; // 00001100

    int b = makeBit1(a, 0);
    int c = makeBit0(a, 3);

    int d1 = makeBit01(a, 0);
    int d2 = makeBit01(d1, 0);

    std::bitset<8> bitsA(a);
    std::bitset<8> bitsB(b);
    std::bitset<8> bitsC(c);
    std::bitset<8> bitsD1(d1);
    std::bitset<8> bitsD2(d2);

    // std::bitset<8> bitMinusA(-12);
    // std::cout << bitMinusA << std::endl;
    // std::cout << std::bitset<8>(-6) << std::endl;

    std::cout << "Bits A: " << bitsA << std::endl;
    std::cout << "Bits B: " << bitsB << std::endl;
    std::cout << "Bits C: " << bitsC << std::endl;
    std::cout << "Bits D1: " << bitsD1 << std::endl;
    std::cout << "Bits D2: " << bitsD2 << std::endl;

    return 0;
}
