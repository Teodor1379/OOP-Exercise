#ifndef MIX_SOUND_H
#define MIX_SOUND_H

#include "Sound.hpp"
#include "SoundContainer.hpp"

template<typename T>
class MixSound: public Sound<T> {
public:
    MixSound(const T& limit, const SoundContainer<T>& sounds);

    virtual bool isEffect()     const override { return false; }
    virtual bool isTransition() const override { return false; }

    virtual Sound<T>* clone() const override;

    virtual ~MixSound() = default;
private:
    void build(const SoundContainer<T>& sounds);

    std::size_t findMaxDuration(const SoundContainer<T>& sounds) const;
};

template<typename T>
inline MixSound<T>::MixSound(const T& limit, const SoundContainer<T>& sounds): Sound<T>(limit, 0) {
    if (!SoundContainer<T>::validateSoundContainer(sounds)) {
        throw std::invalid_argument("Invalid arguments for the mix sound!");
    }

    this->build(sounds);
}

template<typename T>
inline Sound<T>* MixSound<T>::clone() const {
    return new MixSound(*this);
}

template<typename T>
inline void MixSound<T>::build(const SoundContainer<T>& sounds) {
    this->duration = this->findMaxDuration(sounds);

    for (std::size_t i = 0; i < this->duration; ++i) {
        T sumValues = 0;

        for (std::size_t j = 0; j < sounds.getSize(); ++j) {
            if (i < sounds[j]->getDuration()) {
                sumValues = sumValues + (*sounds[j])[i];
            }
        }

        this->samples.push_back(Sample<T>(sumValues / sounds.getSize(), this->limit));
    }
}

template<typename T>
inline std::size_t MixSound<T>::findMaxDuration(const SoundContainer<T>& sounds) const {
    std::size_t max = sounds[0]->getDuration();

    for (std::size_t i = 1; i < sounds.getSize(); ++i) {
        if (max < sounds[i]->getDuration()) {
            max = sounds[i]->getDuration();
        }
    }

    return max;
}

#endif
