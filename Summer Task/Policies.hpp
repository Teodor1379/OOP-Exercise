#ifndef POLICIES_H
#define POLICIES_H

#include <exception>
#include <stdexcept>

template<typename T>
struct AmplifyPolicy {
    AmplifyPolicy(const double& coefficient) {
        if (coefficient < 0.0) {
            throw std::invalid_argument("Invalid coefficient for the amplify policy!");
        }

        this->coefficient = coefficient;
    }
    
    double  coefficient = 1.0;

    Sample<T> operator()(const Sample<T>& sample, const std::size_t& index) {
        return Sample<T>(sample.getValue() * coefficient, sample.getLimit());
    }
};

template<typename T>
struct InvertPolicy {
    Sample<T> operator()(const Sample<T>& sample, const std::size_t& index) {
        return Sample<T>(-sample.getValue(), sample.getLimit());
    }
};

template<typename T>
struct NormalizePolicy {
    NormalizePolicy(const Sound<T>& sound, const T& threshold, bool changeDefault = false) {
        this->amplitude = this->getMaxValue(sound);
        this->threshold = (changeDefault) ? threshold : sound.getLimit();
    }

    T   amplitude;  // Maximal sample value in a sound.
    T   threshold;  // The wanted threshold for a sound.

    Sample<T> operator()(const Sample<T>& sample, const std::size_t& index) {
        return Sample<T>(sample.getValue() * threshold / amplitude, sample.getLimit());
    }

private:
    T getMaxValue(const Sound<T>& sound) const {
        T maxValue = sound[0];

        for (std::size_t i = 0; i < sound.getDuration(); ++i) {
            if (maxValue < sound[i]) {
                maxValue = sound[i];
            }
        }

        return maxValue;
    }
};

template<typename T>
struct FadeInPolicy {
    FadeInPolicy(const std::size_t& duration, const std::size_t& length, const double& level) {
        if (length > duration) {
            throw std::invalid_argument("Length for the fade in is too long!");
        }

        this->duration  = duration;
        this->length    = length;
        this->level     = level;
    }

    std::size_t duration;   // The length of the whole sound.
    std::size_t length;     // The length of the fade in.
    double      level;      // The level of the sound.

    // Example for the policy: For 5 samples, fade in to level 1.0. This is 1.0 / 5 = 0.2 added for sample.
    // Samples are:         1       2       3       4       5       6       7       8       9       10
    // Sample levels are:   0.2     0.4     0.6     0.8     1.0     1.0     1.0     1.0     1.0     1.0
    // New samples are:     0.2     0.8     1.8     3.2     5.0     6.0     7.0     8.0     9.0     10.0

    Sample<T> operator()(const Sample<T>& sample, const std::size_t& index) {
        if (index + 1 > length) {
            return Sample<T>(sample.getValue() * this->level, sample.getLimit());
        } else {
            return Sample<T>(sample.getValue() * (index + 1) * (level / length), sample.getLimit());
        }
    }
};

template<typename T>
struct FadeOutPolicy {
    FadeOutPolicy(const std::size_t& duration, const std::size_t& length, const double& level, const std::size_t& offset) {
        if (length > duration || offset > duration - length) {
            throw std::invalid_argument("Length for the fade out is too long!");
        }

        this->duration      = duration;
        this->length        = length;

        this->level         = level;
        
        this->start         = duration - offset - length;
        this->counter       = 1;
    }

    std::size_t duration;   // The length of the whole sound.
    std::size_t length;     // The length of the fade out.

    double level;           // The level for the fade out.
    
    std::size_t start;      // The start index (used in case of offset).
    std::size_t counter;    // An instance counter for calculations.

    // Example for the policy: For 2 samples, fade in to level 0.5. This is 0.5 / 2 = 0.25 added for sample.
    // Samples are:         1       2       3       4       5       6       7       8       9       10
    // Sample levels are:   1.0     1.0     1.0     1.0     1.0     1.0     1.0     1.0     0.75    0.5
    // New samples are:     1.0     2.0     3.0     4.0     5.0     6.0     7.0     8.0     6.75    5.0

    // Example for the policy: For 4 samples, 2 samples fade in to level 0.5, then to 0.0.
    // Samples are:         1       2       3       4       5       6       7       8       9       10
    // Sample levels are:   1.0     1.0     1.0     1.0     1.0     1.0     0.75    0.5     0.25    0.0
    // New samples are:     1.0     2.0     3.0     4.0     5.0     6.0     5.25    4.0     2.25    0.0

    // Example for the policy: For 4 samples, 2 samples fade in to level 0.3, then to 0.0.
    // Samples are:         1       2       3       4       5       6       7       8       9       10
    // Sample levels are:   1.0     1.0     1.0     1.0     1.0     1.0     0.65    0.3     0.15    0.0
    // New samples are:     1.0     2.0     3.0     4.0     5.0     6.0     4.55    2.4     1.35    0.0

    // Example for the policy: For 4 samples, 2 samples fade in to level 0.3, then to 0.0, amplify 2.0.
    // Samples are:         1       2       3       4       5       6       7       8       9       10
    // After Amplify:       2.0     4.0     6.0     8.0     10.0    12.0    14.0    16.0    18.0    20.0
    // Sample levels are:   1.0     1.0     1.0     1.0     1.0     1.0     0.65    0.3     0.15    0.0
    // New samples are:     2.0     4.0     6.0     8.0     10.0    12.0    9.10    4.8     2.7     0.0

    Sample<T> operator()(const Sample<T>& sample, const std::size_t& index) {
        if (index < this->start) {
            return Sample<T>(sample.getValue(), sample.getLimit());
        } else if (this->counter > this->length) {
            T coefficient = this->level - (this->counter - this->length) * level / (this->duration - this->length - this->start);
            this->counter = this->counter + 1;
            return Sample<T>(sample.getValue() * coefficient, sample.getLimit());
        } else {
            T coefficient = level + (length - counter) * (1.0 - level) / length;
            this->counter = this->counter + 1;
            return Sample<T>(sample.getValue() * coefficient, sample.getLimit());
        }
    }
};

#endif
