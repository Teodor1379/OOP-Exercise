#ifndef TRANSITION_H
#define TRANSITION_H

#include "SoundContainer.hpp"
#include "MixSound.hpp"

#include "Effect.hpp"
#include "EffectManager.hpp"

#include <exception>
#include <stdexcept>

template<typename T>
class Transition: public MixSound<T> {
public:
    Transition(
        const T& limit, const Sound<T>& sound1, const Sound<T>& sound2,
        const std::size_t& samples1, const std::size_t& samples2,
        const double& amplify1, const double& amplify2, const double& level
    );

    double getAmplify1()    const { return this->amplify1; }
    double getAmplify2()    const { return this->amplify2; }

    std::size_t getSamples1()   const { return this->samples1; }
    std::size_t getSamples2()   const { return this->samples2; }

    double getLevel()   const { return this->level; }

    virtual bool isTransition() const override { return true; }

    virtual Sound<T>* clone() const override {
        return new Transition<T>(*this);
    }

    virtual ~Transition() = default;

private:
    static SoundContainer<T> buildSoundContainer(
        const T& limit, const Sound<T>& sound1, const Sound<T>& sound2,
        const std::size_t& samples1, const std::size_t& samples2,
        const double& amplify1, const double& amplify2, const double& level) {

        if (samples1 < samples2 || amplify1 < 0.0 || amplify2 < 0.0 || level < 0.0) {
            throw std::invalid_argument("Invalid arguments for the transition");
        }

        Vector<T> vector1 = sound1.getValues();
        Vector<T> vector2 = sound2.getValues();

        PeriodicalSound<T> temporary1(limit, vector1, 1);
        PeriodicalSound<T> temporary2(limit, vector2, 1);

        Effect<T, AmplifyPolicy<T>> effect1 = EffectManager<T>::buildAmplifyEffect(temporary1, amplify1);
        Effect<T, FadeOutPolicy<T>> songFadeOut = EffectManager<T>::buildFadeOutEffect(effect1, samples1 - samples2, level, samples2);
        Effect<T, AmplifyPolicy<T>> effect2 = EffectManager<T>::buildAmplifyEffect(temporary2, amplify2);
        Effect<T, FadeInPolicy<T>> songFadeIn = EffectManager<T>::buildFadeInEffect(effect2, samples2, 1.0);

        Vector<T> array1 = Transition<T>::buildVector1(songFadeOut, songFadeIn, samples2);
        Vector<T> array2 = Transition<T>::buildVector2(songFadeOut, songFadeIn, samples2);

        PeriodicalSound<T> periodicalSound1(limit * 2, array1, 1);
        PeriodicalSound<T> periodicalSound2(limit * 2, array2, 1);

        SoundContainer<T> soundContainer;
        
        soundContainer.push_back(periodicalSound1);
        soundContainer.push_back(periodicalSound2);

        return soundContainer;
    }

    static Vector<T> buildVector1(const Sound<T>& songFadeOut, const Sound<T>& songFadeIn, const std::size_t& samples) {
        Vector<T> array1;

        for (std::size_t i = 0; i < songFadeOut.getDuration() + songFadeIn.getDuration() - samples; ++i) {
            if (i < songFadeOut.getDuration() - samples) {
                array1.push_back(songFadeOut[i] * 2);
            } else if (i >= songFadeOut.getDuration() - samples && i < songFadeOut.getDuration()) {
                array1.push_back(songFadeOut[i]);
            } else {
                array1.push_back(0);
            }
        }

        return array1;
    }

    static Vector<T> buildVector2(const Sound<T>& songFadeOut, const Sound<T>& songFadeIn, const std::size_t& samples) {
        Vector<T> array2;

        std::size_t counter = 0;

        for (std::size_t i = 0; i < songFadeOut.getDuration() + songFadeIn.getDuration() - samples; ++i) {
            if (i < songFadeOut.getDuration() - samples) {
                array2.push_back(0);
            } else if (i >= songFadeOut.getDuration() - samples && i < songFadeOut.getDuration()) {
                array2.push_back(songFadeIn[counter]);
                counter = counter + 1;
            } else {
                array2.push_back(songFadeIn[counter] * 2);
                counter = counter + 1;
            }
        }

        return array2;
    }

    double amplify1;
    double amplify2;

    std::size_t samples1;
    std::size_t samples2;

    double level;
};

template<typename T>
inline Transition<T>::Transition(
    const T& limit, const Sound<T>& sound1, const Sound<T>& sound2,
    const std::size_t& samples1, const std::size_t& samples2,
    const double& amplify1, const double& amplify2, const double& level):
MixSound<T>(limit, Transition<T>::buildSoundContainer(limit, sound1, sound2, samples1, samples2, amplify1, amplify2, level)) {
    this->amplify1  = amplify1;
    this->amplify2  = amplify2;
    
    this->samples1  = samples1;
    this->samples2  = samples2;

    this->level     = level;
}

#endif
