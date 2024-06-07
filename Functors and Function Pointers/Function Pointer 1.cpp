#include <cstdlib>

#include <iostream>

int x = 10;
int y = 5;

int addition        (int a, int b)  { return a + b; }
int substraction    (int a, int b)  { return a - b; }
int multiplication  (int a, int b)  { return a * b; }
int division        ()              { return x / y; }

void print(int (*funcPointer)()) {
    std::cout << funcPointer() << std::endl;
}

int main() {
    int (*funcPointer1)(int, int);
    int (*funcPointer2)(int, int);
    int (*funcPointer3)(int, int);

    funcPointer1 = addition;
    funcPointer2 = substraction;
    funcPointer3 = multiplication;

    std::cout << funcPointer1(10, 20) << std::endl;
    std::cout << funcPointer2(30, 20) << std::endl;
    std::cout << funcPointer3(23, 3) << std::endl;

    std::cout << std::endl;

    print(division);

    return 0;
}
