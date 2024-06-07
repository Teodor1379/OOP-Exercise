#ifndef PERIODICAL_SOUND_H
#define PERIODICAL_SOUND_H

#include "Sound.hpp"

#include <exception>
#include <stdexcept>

template<typename T>
class PeriodicalSound: public Sound<T> {
public:
    PeriodicalSound(const T& limit, const Vector<T>& values, const std::size_t& number);

    virtual bool isEffect()     const override { return false; }
    virtual bool isTransition() const override { return false; }

    virtual Sound<T>* clone() const override;

    virtual ~PeriodicalSound() = default;

private:
    void build(const Vector<T>& values, const std::size_t& number);
};

template<typename T>
inline PeriodicalSound<T>::PeriodicalSound(const T& limit, const Vector<T>& values, const std::size_t& number): Sound<T>(limit, 0) {
    if (values.getSize() == 0 || number == 0) {
        throw std::invalid_argument("Invalid arguments for the periodical sound!");
    }

    this->build(values, number);
}

template<typename T>
inline Sound<T>* PeriodicalSound<T>::clone() const {
    return new PeriodicalSound<T>(*this);
}

template<typename T>
inline void PeriodicalSound<T>::build(const Vector<T>& values, const std::size_t& number) {
    for (std::size_t i = 1; i <= number; ++i) {
        for (std::size_t j = 0; j < values.getSize(); ++j) {
            this->samples.push_back(Sample<T>(values[j], this->limit));
        }
    }

    this->duration = this->samples.getSize();
}

#endif
