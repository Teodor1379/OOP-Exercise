#include <cstdlib>

#include <iostream>

int compare(const void* number1, const void* number2) {
    const int* x = (int*) number1;
    const int* y = (int*) number2;

    if (*x > *y) {
        return 1;
    } else if (*x < *y) {
        return -1;
    } else {
        return 0;
    }
}

int main() {
    const int size = 5;

    int array[size] = { 5, 4, 3, 2, 1 };

    qsort(array, size, sizeof(int), compare);

    for (int i = 0; i < size; ++i) {
        std::cout << array[i] << ' ';
    }

    std::cout << std::endl;

    return 0;
}
