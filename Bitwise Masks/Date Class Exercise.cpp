/*

    Date - клас, който представя валидна дата след 01.01.1970г. Класът трябва да:
    
    е оптимален откъм памет - чрез побитови операции и маски, трябва да съхраните датата в 16 бита по следната схема:
    5 бита, които да пазят стойността на дните (0-31)
    4 бита, които да пазят стойността на месеците (0-15)
    6 бита, които да пазят стойността на годините (0-63)

    реализира метод print, който извежда дата в посочения по-горе формат на подаден като параметър текстов поток
    има подходящи методи за достъп до данните.

*/

#include <bitset>

#include <iostream>

class Date {
public:
    /*
        Date(unsigned short day, unsigned short month, unsigned short year) {
            // Credits: Andy

            date = 0;
            date |= year;
            date <<= 4;
            date |= month;
            date <<= 5;
            date |= day;

            std::bitset<16> bits(this->date);

            std::cout << bits << std::endl;
        }
    */

    Date(unsigned short day, unsigned short month, unsigned short year) {
        this->date = 0; 

        this->date |= (year << 9);
        this->date |= (month << 5);
        this->date |= day;

        // 0010101010001010

        std::bitset<16> bits(this->date);

        std::cout << bits << std::endl;
    }

    uint16_t getDay() { return this->date & 0b11111; }
    uint16_t getMonth() { return (this->date >> 5) & 0b1111; }
    uint16_t getYear() { return (this->date >> 9) & 0b111111; }

    void print() {
        std::cout << "Day: " << this->getDay() << std::endl;
        std::cout << "Month: " << this->getMonth() << std::endl;
        std::cout << "Year: " << this->getYear() << std::endl;
    }

private:
    // unsigned short - Credits: Jessi

    uint16_t date;
};

int main() {
    Date date = Date(10, 11, 20);

    date.print();
    
    return 0;
}