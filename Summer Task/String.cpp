#include "String.hpp"

#include <cstring>

#include <exception>
#include <stdexcept>

String& String::operator=(const String& instance) {
    if (this != &instance) {
        char* copy = new char[strlen(instance.string) + 1];
        strcpy(copy, instance.string);

        delete[] this->string;
        this->string = copy;
    }

    return *this;
}

char& String::operator[](const std::size_t pos) {
    if ((strlen(this->string) == 0 && pos > 0) || (pos > strlen(this->string) - 1)) {
        throw std::out_of_range("Invalid index for the string!");
    }

    return this->string[pos];
}

const char& String::operator[](const std::size_t pos) const {
    if ((strlen(this->string) == 0 && pos > 0) || (pos > strlen(this->string) - 1)) {
        throw std::out_of_range("Invalid index for the string!");
    }

    return this->string[pos];
}

void String::removeSpaces() {
    std::size_t index1 = 0;
    std::size_t index2 = 0;
    bool condition = false;

    for (int i = 0; this->string[0] != '\0'; ++i) {
        if (this->string[i] != ' ') {
            index1 = i; break;
        }
    }

    for (int i = strlen(this->string) - 1; i >= 0; --i) {
        if (this->string[i] != ' ') {
            index2 = i; break;
        }
    }

    for (std::size_t i = 1; this->string[i] != '\0'; ++i) {
        if (this->string[i] == ' ' && this->string[i - 1] == ' ') {
            condition = true; break;
        }
    }

    if (index1 == index2 || !condition) {
        return;
    }

    char* current = new char[strlen(this->string) + 1];

    std::size_t counterLength = 0;

    for (std::size_t i = index1; i <= index2; ++i) {
        if (this->string[i] == ' ') {
            while (this->string[i + 1] == ' ') {
                i = i + 1;
            }
        }

        current[counterLength] = this->string[i];
        counterLength = counterLength + 1;
    }

    current[counterLength] = '\0';

    delete[] this->string;
    this->string = current;
}

std::size_t String::countSpaces() const {
    std::size_t spaces = 0;

    for (std::size_t i = 0; this->string[i] != '\0'; ++i) {
        if (this->string[i] == ' ') {
            spaces = spaces + 1;
        }
    }

    return spaces;
}

bool String::isNumber() const {
    for (std::size_t i = 0; this->string[i] != '\0'; ++i) {
        bool condition1 = (this->string[i] >= 'a' && this->string[i] <= 'z' && this->string[i] != 'e');
        bool condition2 = (this->string[i] >= 'A' && this->string[i] <= 'Z' && this->string[i] != 'E');
        bool condition3 = (this->string[i] < '0' || this->string[i] > '9');
        bool condition4 = (this->string[i] != '+' && this->string[i] != '-' && this->string[i] != ' ');
        bool condition5 = (this->string[i] != '.' && this->string[i] != 'e' && this->string[i] != 'E');

        if (condition1 || condition2 || (condition3 && condition4 && condition5)) {
            return false;
        }
    }

    return true;
}

void String::build(char*& destination, const char* source) {
    if (source == nullptr) {
        throw std::invalid_argument("Invalid string argument!");
    }

    destination = new char[strlen(source) + 1];
    strcpy(destination, source);
}
