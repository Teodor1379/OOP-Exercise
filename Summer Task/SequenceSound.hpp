#ifndef SEQUENCE_SOUND_H
#define SEQUENCE_SOUND_H

#include "Sound.hpp"
#include "SoundContainer.hpp"

#include <exception>
#include <stdexcept>

template<typename T>
class SequenceSound: public Sound<T> {
public:
    SequenceSound(const T& limit, const SoundContainer<T>& sounds);

    virtual bool isEffect()     const override { return false; }
    virtual bool isTransition() const override { return false; }

    virtual Sound<T>* clone() const override;

    virtual ~SequenceSound() = default;

private:
    void build(const SoundContainer<T>& sounds);

    std::size_t getSumDurations(const SoundContainer<T>& sounds) const;
};

template<typename T>
inline SequenceSound<T>::SequenceSound(const T& limit, const SoundContainer<T>& sounds): Sound<T>(limit, 0) {
    if (!SoundContainer<T>::validateSoundContainer(sounds)) {
        throw std::invalid_argument("Invalid arguments for the sequence sound!");
    }

    this->build(sounds);
}

template<typename T>
inline Sound<T>* SequenceSound<T>::clone() const {
    return new SequenceSound<T>(*this);
}

template<typename T>
inline void SequenceSound<T>::build(const SoundContainer<T>& sounds) {
    this->duration = this->getSumDurations(sounds);

    for (std::size_t i = 0; i < sounds.getSize(); ++i) {
        for (std::size_t j = 0; j < sounds[i]->getDuration(); ++j) {
            this->samples.push_back(Sample<T>((*sounds[i])[j], this->limit));
        }
    }
}

template<typename T>
inline std::size_t SequenceSound<T>::getSumDurations(const SoundContainer<T>& sounds) const {
    std::size_t sum = 0;

    for (std::size_t i = 0; i < sounds.getSize(); ++i) {
        sum +=  sounds[i]->getDuration();
    }

    return sum;
}

#endif
