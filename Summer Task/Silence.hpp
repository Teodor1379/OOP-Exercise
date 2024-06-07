#ifndef SILENCE_H
#define SILENCE_H

#include "Sound.hpp"

template<typename T>
class Silence: public Sound<T> {
public:
    Silence(const T& limit, const std::size_t& duration);

    virtual bool isEffect()     const { return false; }
    virtual bool isTransition() const { return false; }

    virtual Sound<T>* clone() const override;

    virtual ~Silence() = default;

private:
    void build();
};

template<typename T>
inline Silence<T>::Silence(const T& limit, const std::size_t& duration)
: Sound<T>(limit, duration) { this->build(); }

template<typename T>
inline Sound<T>* Silence<T>::clone() const {
    return new Silence<T>(*this);
}

template<typename T>
inline void Silence<T>::build() {
    for (std::size_t i = 0; i < this->duration; ++i) {
        this->samples.push_back(Sample<T>(0, this->limit));
    }
}

#endif
