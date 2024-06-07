#ifndef SOUND_H
#define SOUND_H

#include "Vector.hpp"
#include "Sample.hpp"

#include <iostream>
#include <fstream>

#include <exception>
#include <stdexcept>

template<typename T>
class Sound {
public:
    Sound();
    Sound(const T& limit, const std::size_t& duration);

    Sound<T>& operator=(const Sound<T>& instance) = delete;

    const T operator[](const std::size_t index) const;

    T getLimit()                            const { return this->limit;     }
    std::size_t getDuration()               const { return this->duration;  }
    const Vector<Sample<T>> getSamples()    const { return this->samples;   }

    virtual bool isEffect()     const { return false; }
    virtual bool isTransition() const { return false; }

    void save(const char* filePath) const;

    Vector<T> getValues()       const;

    virtual Sound<T>* clone() const = 0;

    virtual ~Sound() = default;

    friend std::ostream& operator<<(std::ostream& stream, const Sound<T>& sound) {
        stream << "The limit of the sound is: "     << sound.limit      << '\n';
        stream << "The duration of the sound is: "  << sound.duration   << '\n';

        if (sound.duration > 0) {
            stream << "The samples values in the sound are: ";

            for (std::size_t i = 0; i < sound.duration; ++i) {
                stream << sound[i] << ' ';
            }
        } else {
            stream << "The sound has no samples within itself.";
        }

        return stream;
    }

protected:
    T                   limit;
    std::size_t         duration;
    Vector<Sample<T>>   samples;
};

template<typename T>
inline Sound<T>::Sound() {
    this->limit     = 0;
    this->duration  = 0;
    this->samples   = Vector<Sample<T>>();
}

template<typename T>
inline Sound<T>::Sound(const T& limit, const std::size_t& duration) {
    this->limit     = limit;
    this->duration  = duration;
    this->samples   = Vector<Sample<T>>();
}

template<typename T>
inline const T Sound<T>::operator[](const std::size_t index) const {
    if (index >= this->duration) {
        throw std::out_of_range("Invalid index for the sample in the sound!");
    }
    
    return this->samples[index].getValue();
}

template<typename T>
inline void Sound<T>::save(const char* filePath) const {
    std::ofstream fileStream(filePath, std::ios::binary | std::ios::out);

    if (!fileStream.is_open()) {
        throw std::runtime_error("Problem while opening the file!");
    }

    for (std::size_t i = 0; i < this->duration; ++i) {
        T value = (*this)[i];
        fileStream.write(reinterpret_cast<const char*>(&value), sizeof(T));
    }

    fileStream.close();
}

template<typename T>
inline Vector<T> Sound<T>::getValues() const {
    Vector<T> values;

    for (std::size_t i = 0; i < this->duration; ++i) {
        values.push_back(this->samples[i].getValue());
    }

    return values;
}

#endif
