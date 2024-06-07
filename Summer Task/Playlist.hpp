#ifndef PLAYLIST_H
#define PLAYLIST_H

#include "Vector.hpp"

#include "Sound.hpp"
#include "FileSound.hpp"

#include "Effect.hpp"
#include "EffectManager.hpp"

#include "Transition.hpp"

#include <exception>
#include <stdexcept>

template<typename T>
class Playlist {
public:
    Playlist(const T& limit, const char* filePath);
    Playlist(const Playlist<T>& playlist);

    Playlist<T>& operator=(const Playlist& playlist);

    T getLimit()            const { return this->limit;     }
    String getFilePath()    const { return this->filePath;  }
    std::size_t getLength() const { return this->length;    }

    explicit operator Sound<T>&();

    Sound<T>& operator[](const std::size_t& index);
    const Sound<T>& operator[](const std::size_t& index) const;

    void replaceSound(const std::size_t& index, const Sound<T>& sound);
    void save(const char* filePath);

    ~Playlist() { delete this->sound; }

private:
    bool validateFileSize() const;

    Vector<char> buildSoundTypes();
    void buildSoundContainer(const Vector<char>& soundTypes);

    void replaceTransition(const std::size_t& index, const Transition<T>& transition);
    void changeTransitions(const std::size_t& index);

    T                   limit;
    String              filePath;
    std::size_t         length;
    Sound<T>*           sound;

    Vector<std::size_t> soundIndices;
    Vector<std::size_t> skippedIndices;
    SoundContainer<T>   soundContainer;
};

template<typename T>
inline Playlist<T>::Playlist(const T& limit, const char* filePath): limit(limit), filePath(filePath), length(0), sound(nullptr) {
    if (!this->validateFileSize()) {
        throw std::invalid_argument("There is a line with more than 1000 characters!");
    }

    Vector<char> vector = this->buildSoundTypes();
    this->buildSoundContainer(vector);
}

template<typename T>
inline Playlist<T>::Playlist(const Playlist<T>& playlist) {
    this->limit             = playlist.limit;
    this->filePath          = playlist.filePath;
    this->length            = playlist.length;

    this->sound             = nullptr;

    this->soundIndices      = playlist.soundIndices;
    this->skippedIndices    = playlist.skippedIndices;
    this->soundContainer    = playlist.soundContainer;
}

template<typename T>
inline Playlist<T>& Playlist<T>::operator=(const Playlist<T>& playlist) {
    if (this != &playlist) {
        delete this->sound;

        this->limit             = playlist.limit;
        this->filePath          = playlist.filePath;
        this->length            = playlist.length;

        this->sound             = nullptr;

        this->soundIndices      = playlist.soundIndices;
        this->skippedIndices    = playlist.skippedIndices;
        this->soundContainer    = playlist.soundContainer;
    }

    return *this;
}

template<typename T>
inline Playlist<T>::operator Sound<T>&() {
    Vector<T> array;

    for (std::size_t i = 0; i < this->soundContainer.getSize(); ++i) {
        if (i == 0 && this->soundContainer[i + 1]->isTransition()) {
            continue;
        } else if (i == this->soundContainer.getSize() - 1 && this->soundContainer[i - 1]->isTransition()) {
            continue;
        } else if (i > 0 && i < this->soundContainer.getSize() - 1) {
            bool condition1 = this->soundContainer[i - 1]->isTransition();
            bool condition2 = this->soundContainer[i + 1]->isTransition();

            if (condition1 || condition2) { continue; }
        }
        
        bool skipped = false;

        for (std::size_t k = 0; k < this->skippedIndices.getSize(); ++k) {
            if (i == this->skippedIndices[k]) {
                skipped = true;
                break;
            }
        }

        if (!skipped) {
            for (std::size_t j = 0; j < this->soundContainer[i]->getDuration(); ++j) {
                array.push_back((*this->soundContainer[i])[j]);
            }
        }
    }

    if (this->sound != nullptr) { delete this->sound; }
    this->sound = new PeriodicalSound<T>(this->limit, array, 1);
    return *this->sound;
}

template<typename T>
inline Sound<T>& Playlist<T>::operator[](const std::size_t& index) {
    if (index >= this->length) {
        throw std::out_of_range("Invalid index for the playlist!");
    }

    return *this->soundContainer[this->soundIndices[index]];
}

template<typename T>
inline const Sound<T>& Playlist<T>::operator[](const std::size_t& index) const {
    if (index >= this->length) {
        throw std::out_of_range("Invalid index for the playlist!");
    }

    return *this->soundContainer[this->soundIndices[index]];
}

template<typename T>
inline void Playlist<T>::replaceSound(const std::size_t& index, const Sound<T>& sound) {
    // If only changing the sound, the user can pass the sound itself
    // in order to the original sound to be replaced by the new one.

    // If applying effect on the sound, the user can construct the
    // effect which he needs with the EffectManager call pass the
    // effect. The effect can take the original sound or take a new
    // one, which is not even in the playlist.

    this->soundContainer.replace(this->soundIndices[index], sound);
    changeTransitions(this->soundIndices[index]);
}

template<typename T>
inline void Playlist<T>::save(const char* filePath) {
    if (filePath == nullptr) {
        throw std::invalid_argument("Invalid file path for saving the playlist!");
    }

    Sound<T>& sound = static_cast<Sound<T>&>(*this);
    sound.save(filePath);
}

template<typename T>
inline bool Playlist<T>::validateFileSize() const {
    std::ifstream fileStream(filePath.c_str(), std::ios::in);

    if (!fileStream.is_open()) {
        throw std::runtime_error("Problem while opening the file!");
    }

    unsigned short counter = 0;

    while (!fileStream.eof()) {
        char character = '\0';
        fileStream.get(character);

        if (character == '\n') {
            counter = 0;
        } else {
            counter = counter + 1;
        }

        if (counter > 1000) {
            return false;
        }
    }

    fileStream.close();
    return true;
}

template<typename T>
inline Vector<char> Playlist<T>::buildSoundTypes() {
    std::ifstream fileStream(this->filePath.c_str(), std::ios::in);

    if (!fileStream.is_open()) {
        throw std::runtime_error("Problem while opening the file!");
    }

    Vector<String> stringTypes;
    char line[1001] = { '\0' };

    while (!fileStream.eof()) {
        fileStream.getline(line, 1001);
        stringTypes.push_back(String(line));
    }

    fileStream.close();

    Vector<char> soundTypes;

    for (std::size_t i = 0; i < stringTypes.getSize(); ++i) {
        switch (stringTypes[i].countSpaces()) {
            case 0: soundTypes.push_back('p');  break;
            case 1: soundTypes.push_back('s');  break;
            case 2: {
                if (stringTypes[i].isNumber())  { soundTypes.push_back('o');  }
                else                            { soundTypes.push_back('e');  }
                break;
            }
            default: throw std::runtime_error("Invalid content of the file!");
        }
    }

    if (soundTypes[0] == 'o' || soundTypes[soundTypes.getSize() - 1] == 'o') {
        throw std::runtime_error("Invalid content of the file with the playlist!");
    }

    for (std::size_t i = 1; i < soundTypes.getSize() - 1; ++i) {
        if (soundTypes[i] == 'o') {
            bool condition1 = (soundTypes[i - 1] == 's');
            bool condition2 = (soundTypes[i + 1] == 's');

            if (!condition1 || !condition2) {
                throw std::runtime_error("Invalid content of the file!");
            }
        }
    }

    return soundTypes;
}

template<typename T>
inline void Playlist<T>::buildSoundContainer(const Vector<char>& soundTypes) {
    std::ifstream fileStream(this->filePath.c_str(), std::ios::in);

    if (!fileStream.is_open()) {
        throw std::runtime_error("Problem while opening the file!");
    }

    std::size_t counterIndex = 0;

    bool reservedSong = false;

    std::size_t reservedSize1 = 0;
    std::size_t reservedSize2 = 0;
    double      reservedNumber = 0.0;



    String currentFileName;

    for (std::size_t i = 0; i < soundTypes.getSize(); ++i) {
        switch(soundTypes[i]) {
            case 'p': {
                std::size_t duration; fileStream >> duration;

                this->soundIndices.push_back(i);
                this->length = this->length + 1;
                
                this->soundContainer.push_back(Silence<T>(this->limit, duration));
                counterIndex = counterIndex + 1;
                break;
            }

            case 's': {
                char name[1000];    fileStream >> name;
                double number;      fileStream >> number;

                if (reservedSong) {
                    if (i >= 3 && this->soundContainer[this->soundContainer.getSize() - 2]->isTransition()) {
                        FileSound<T> fileSound(this->limit, name);
                        Effect<T, AmplifyPolicy<T>> effect = EffectManager<T>::buildAmplifyEffect(fileSound, number);

                        this->skippedIndices.push_back(i - 2);
                        this->skippedIndices.push_back(i - 3);

                        Transition<T> transition(
                            this->limit,
                            *this->soundContainer[this->soundContainer.getSize() - 2], effect,
                            reservedSize1, reservedSize2,
                            1.0, 1.0, reservedNumber
                        );
                        
                        this->soundContainer.push_back(transition);
                        this->soundContainer.push_back(effect);

                    } else {
                        FileSound<T> fileSound(this->limit, name);
                        Effect<T, AmplifyPolicy<T>> effect = EffectManager<T>::buildAmplifyEffect(fileSound, number);

                        Transition<T> transition(
                            this->limit,
                            *this->soundContainer[this->soundContainer.getSize() - 1], effect,
                            reservedSize1, reservedSize2,
                            1.0, 1.0, reservedNumber
                        );

                        this->soundContainer.push_back(transition);
                        this->soundContainer.push_back(effect);
                    }
                    
                    reservedSong = false;
                } else {
                    FileSound<T> fileSound(this->limit, name);
                    Effect<T, AmplifyPolicy<T>> effect = EffectManager<T>::buildAmplifyEffect(fileSound, number);
                    this->soundContainer.push_back(effect);

                    currentFileName = String(name);      
                }

                this->soundIndices.push_back(i);
                this->length = this->length + 1;
                counterIndex = counterIndex + 1;

                break;
            }

            case 'e': {
                char name[1000];        fileStream >> name;
                std::size_t duration;   fileStream >> duration;
                double number;          fileStream >> number;

                this->soundIndices.push_back(i);
                this->length = this->length + 1;

                FileSound<T> fileSound(this->limit, name);
                Effect<T, FadeInPolicy<T>> effect = EffectManager<T>::buildFadeInEffect(fileSound, duration, number);
                this->soundContainer.push_back(effect);
                counterIndex = counterIndex + 1;

                break;
            }

            case 'o': {
                fileStream >> reservedSize1;
                fileStream >> reservedSize2;
                fileStream >> reservedNumber;

                reservedSong = true;
                break;
            }
        }
    }

    fileStream.close();
}

template<typename T>
inline void Playlist<T>::replaceTransition(const std::size_t& index, const Transition<T>& transition) {
    this->soundContainer.replace(index, transition);
}

template<typename T>
inline void Playlist<T>::changeTransitions(const std::size_t& index) {
    for (std::size_t i = 0; i < this->soundContainer.getSize(); ++i) {
        if (i >= 1 && i <= this->soundContainer.getSize() - 1) {
            if (i > 1 && this->soundContainer[i - 2]->isTransition()) {
                if (i == this->soundContainer.getSize() - 2) {
                    const Transition<T>* transition = dynamic_cast<const Transition<T>*>(this->soundContainer[i - 2]);

                    replaceTransition(i, Transition<T>(
                        this->limit,
                        *transition, *this->soundContainer[i + 1],
                        transition->getSamples1(), transition->getSamples2(),
                        transition->getAmplify1(), transition->getAmplify2(),
                        transition->getLevel()
                    ));
                } else {
                    const Transition<T>* transition = dynamic_cast<const Transition<T>*>(this->soundContainer[i - 2]);

                    replaceTransition(i, Transition<T>(
                        this->limit,
                        *this->soundContainer[i - 2], *this->soundContainer[i - 1],
                        transition->getSamples1(), transition->getSamples2(),
                        transition->getAmplify1(), transition->getAmplify2(),
                        transition->getLevel()
                    ));
                }
            } else {
                if (i >= 1 && i <= this->soundContainer.getSize() - 1) {
                    if (this->soundContainer[i]->isTransition() && (index == i - 1 || index == i + 1)) {
                        const Transition<T>* transition = dynamic_cast<const Transition<T>*>(this->soundContainer[i]);

                        replaceTransition(i, Transition<T>(
                            this->limit,
                            *this->soundContainer[i - 1], *this->soundContainer[i + 1],
                            transition->getSamples1(), transition->getSamples2(),
                            transition->getAmplify1(), transition->getAmplify2(),
                            transition->getLevel()
                        ));
                    }
                }
            }
        }
    }
}

#endif
