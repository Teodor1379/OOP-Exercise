#ifndef SOUND_CONTAINER_H
#define SOUND_CONTAINER_H

#include "Sound.hpp"
#include "Silence.hpp"
#include "FileSound.hpp"
#include "PeriodicalSound.hpp"

template<typename T>
class SoundContainer {
public:
    SoundContainer();
    SoundContainer(const SoundContainer<T>& instance);

    ~SoundContainer();

    SoundContainer<T>& operator=(const SoundContainer<T>& instance);

    
    Sound<T>* operator[] (const std::size_t& index)             { return this->sounds[index]; } // No-throw guarantee and undefined behaviour in case of invalid index.  
    const Sound<T>* operator[](const std::size_t& index) const  { return this->sounds[index]; } // No-throw guarantee and undefined behaviour in case of invalid index.

    void replace(std::size_t index, const Sound<T>& sound) {
        if (index >= this->size) {
            throw std::out_of_range("Invalid index for the replacement!");
        }

        delete this->sounds[index];
        this->sounds[index] = sound.clone();
    }

    std::size_t getSize()       const noexcept { return this->size;      }
    std::size_t getCapacity()   const noexcept { return this->capacity;  }

    void push_back(const Sound<T>& sound);
    void pop_back();

    static bool validateSoundContainer(const SoundContainer<T>& sounds) {
        if (sounds.getSize() == 0) {
            return false;
        }

        for (std::size_t i = 0; i < sounds.getSize(); ++i) {
            if (sounds[i]->getDuration() == 0) {
                return false;
            }
        }

        return true;
    }

private:
    void allocate(const SoundContainer<T>& instance);
    void deallocate();

    void pushSound(Sound<T>* sound);
    void resize();

    Sound<T>**  sounds;
    std::size_t size;
    std::size_t capacity;
};

template<typename T>
inline SoundContainer<T>::SoundContainer() {
    this->sounds    = new Sound<T>*[2];
    this->size      = 0;
    this->capacity  = 2;
}

template<typename T>
inline SoundContainer<T>::SoundContainer(const SoundContainer<T>& instance) {
    this->allocate(instance);

    this->size      = instance.size;
    this->capacity  = instance.capacity;
}

template<typename T>
inline SoundContainer<T>::~SoundContainer() {
    this->deallocate();
}

template<typename T>
inline SoundContainer<T>& SoundContainer<T>::operator=(const SoundContainer<T>& instance) {
    if (this != &instance) {        
        this->deallocate();
        this->allocate(instance);

        this->size      = instance.size;
        this->capacity  = instance.capacity;
    }

    return *this;
}

template<typename T>
inline void SoundContainer<T>::push_back(const Sound<T>& sound) {
    this->pushSound(sound.clone());
}

template<typename T>
inline void SoundContainer<T>::pop_back() {
    if (this->size == 0) {
        throw std::runtime_error("The container is empty!");
    }

    delete this->sounds[this->size - 1];
    this->size = this->size - 1;
}

template<typename T>
inline void SoundContainer<T>::allocate(const SoundContainer<T>& instance) {
    this->sounds = new Sound<T>*[instance.capacity];

    for (std::size_t i = 0; i < instance.size; ++i) {
        this->sounds[i] = instance.sounds[i]->clone();
    }
}

template<typename T>
inline void SoundContainer<T>::deallocate() {
    for (std::size_t i = 0; i < this->size; ++i) {
        delete this->sounds[i];
    }

    delete[] this->sounds;
}

template<typename T>
inline void SoundContainer<T>::pushSound(Sound<T>* sound) {
    if (this->size == this->capacity) {
        this->resize();
    }

    this->sounds[this->size] = sound;
    this->size = this->size + 1;
}

template<typename T>
inline void SoundContainer<T>::resize() {
    Sound<T>** temporary = new Sound<T>*[this->capacity * 2];

    for (std::size_t i = 0; i < this->size; ++i) {
        temporary[i] = this->sounds[i];
    }

    delete[] this->sounds;
    this->sounds = temporary;

    this->capacity = this->capacity * 2;
}

#endif
