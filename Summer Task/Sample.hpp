#ifndef SAMPLE_H
#define SAMPLE_H

#include <cmath>

template<typename T>
class Sample {
public:
    Sample();
    Sample(const T& value, const T& limit);

    const T getValue()  const { return this->value; }
    const T getLimit()  const { return this->limit; }

private:
    void clip();

    T   value;
    T   limit;
};

template<typename T>
inline Sample<T>::Sample() {
    this->value = 0;
    this->limit = 0;
}

template<typename T>
inline Sample<T>::Sample(const T& value, const T& limit) {
    this->value = value;
    this->limit = std::abs(limit);

    this->clip();
}

template<typename T>
inline void Sample<T>::clip() {
    if (this->value < -this->limit) {
        this->value = -this->limit;
    } else if (this->value > this->limit) {
        this->value = this->limit;
    }
}

#endif
