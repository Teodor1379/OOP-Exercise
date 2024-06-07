#ifndef FILE_SOUND_H
#define FILE_SOUND_H

#include "String.hpp"
#include "Sound.hpp"

#include <fstream>

template<typename T>
class FileSound: virtual public Sound<T> {
public:
    FileSound(const T& limit, const char* filePath);

    String getFilePath() const { return this->filePath; }

    virtual bool isEffect()     const override { return false; }
    virtual bool isTransition() const override { return false; }

    virtual Sound<T>* clone() const override;

    virtual ~FileSound() = default;

private:
    void build();

    String filePath;
};

template<typename T>
inline FileSound<T>::FileSound(const T& limit, const char* filePath)
: Sound<T>(limit, 0), filePath(filePath) { this->build(); }

template<typename T>
inline Sound<T>* FileSound<T>::clone() const {
    return new FileSound<T>(*this);
}

template<typename T>
inline void FileSound<T>::build() {
    std::ifstream fileStream(this->filePath.c_str(), std::ios::binary | std::ios::in);

    if (!fileStream.is_open()) {
        throw std::runtime_error("Problem while opening the file!");
    }

    T value;

    while (fileStream.read(reinterpret_cast<char*>(&value), sizeof(T))) {
        if (fileStream.good()) {
            this->samples.push_back(Sample<T>(value, this->limit));
        } else {
            fileStream.close();
            throw std::runtime_error("Problem with the file stream!");
        }
    }

    if (fileStream.eof()) {
        // Intentionally empty if statement.
    } else if (fileStream.fail()) {
        throw std::runtime_error("Problem with the file stream!");
    }

    fileStream.close();

    this->duration = this->samples.getSize();
}

#endif
