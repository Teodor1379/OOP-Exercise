#ifndef EFFECT_H
#define EFFECT_H

#include "Vector.hpp"
#include "Sound.hpp"

template<typename T, typename Policy>
class Effect: virtual public Sound<T> {
public:
    Effect(const Sound<T>& sound, const Policy& policy);

    const Policy getPolicy() const { return this->policy; }
    
    virtual bool isEffect()     const override { return true;   }
    virtual bool isTransition() const override { return false;  }

    virtual Sound<T>* clone() const override;

    virtual ~Effect() = default;

private:
    void build(const Sound<T>& sound);

    Policy      policy;
};

template<typename T, typename Policy>
inline Effect<T, Policy>::Effect(const Sound<T>& sound, const Policy& policy):
Sound<T>(sound.getLimit(), sound.getDuration()), policy(policy) { this->build(sound); }

template<typename T, typename Policy>
inline Sound<T>* Effect<T, Policy>::clone() const {
    return new Effect<T, Policy>(*this); 
}

template<typename T, typename Policy>
inline void Effect<T, Policy>::build(const Sound<T>& sound) {
    for (std::size_t i = 0; i < this->duration; ++i) {
        this->samples.push_back(this->policy(sound.getSamples()[i], i));
    }
}

#endif
