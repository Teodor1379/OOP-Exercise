#ifndef EFFECT_MANAGER_H
#define EFFECT_MANAGER_H

#include "Effect.hpp"
#include "Policies.hpp"

// A simple class to directly create objects with simple effects on a sound.

template<typename T>
class EffectManager {
public:
    static Effect<T, AmplifyPolicy<T>> buildAmplifyEffect(const Sound<T>& instance, const double& coefficient) {
        AmplifyPolicy<T> amplifyPolicy = { coefficient };
        return Effect<T, AmplifyPolicy<T>>(instance, amplifyPolicy);
    }

    static Effect<T, InvertPolicy<T>> buildInvertEffect(const Sound<T>& instance) {
        InvertPolicy<T> invertPolicy;
        return Effect<T, InvertPolicy<T>>(instance, invertPolicy);
    }

    static Effect<T, NormalizePolicy<T>> buildNormalizeEffect(const Sound<T>& instance, const T& threshold, bool changeDefault = false) {
        NormalizePolicy<T> normalizePolicy = { instance, threshold, changeDefault};
        return Effect<T, NormalizePolicy<T>>(instance, normalizePolicy);
    }

    static Effect<T, FadeInPolicy<T>> buildFadeInEffect(const Sound<T>& instance, const std::size_t& length, const double& level) {
        FadeInPolicy<T> fadeInPolicy = { instance.getDuration(), length, level };
        return Effect<T, FadeInPolicy<T>>(instance, fadeInPolicy);
    }

    static Effect<T, FadeOutPolicy<T>> buildFadeOutEffect(const Sound<T>& instance,
    const std::size_t& length, const double& level, const std::size_t& offset) {
        FadeOutPolicy<T> fadeOutPolicy = { instance.getDuration(), length, level, offset };
        return Effect<T, FadeOutPolicy<T>>(instance, fadeOutPolicy);
    }
};

#endif
