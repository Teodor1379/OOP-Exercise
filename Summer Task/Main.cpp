// Name: Teodor Dishanski
// Faculty Number: 2MI0400112

#include "Sound.hpp"

#include "Policies.hpp"

#include "FileSound.hpp"
#include "PeriodicalSound.hpp"

#include "Effect.hpp"
#include "EffectManager.hpp"

#include "Playlist.hpp"

#include <exception>
#include <stdexcept>

void constructNormalizationEffectScenario() {
    // I have intentionally made the samples
    // with unrealistic values, so, when we
    // want to make a peak normalization, to
    // see the real output, when we scale them.

    try {
        Vector<double> array;

        array.push_back(200);
        array.push_back(400);
        array.push_back(600);
        array.push_back(800);
        array.push_back(1000);

        PeriodicalSound<double> periodicalSound(1500.0, array, 1);

        Effect<double, NormalizePolicy<double>> effect = EffectManager<double>::buildNormalizeEffect(periodicalSound, 1.0, true);
        std::cout << effect << std::endl;
    } catch (std::exception& exception) {
        std::cerr << exception.what() << std::endl;
    }
}

void buildSimplePlaylistScenario() {
    try {
        Playlist<double> playlist(30.0, "Simple Playlist.txt");

        std::cout << "Every sound in the playlist is: " << std::endl;

        for (std::size_t i = 0; i < playlist.getLength(); ++i) {
            std::cout << playlist[i] << std::endl;
            std::cout << std::endl;
        }

        Sound<double>& sound = static_cast<Sound<double>&>(playlist);
        std::cout << sound << std::endl;
    } catch (std::exception& exception) {
        std::cerr << exception.what() << std::endl;
    }
}

void buildAveragePlaylistScenario() {
    try {
        Playlist<double> playlist(30.0, "Average Playlist.txt");
        Sound<double>& sound = static_cast<Sound<double>&>(playlist);
        std::cout << sound << std::endl;
    } catch (std::exception& exception) {
        std::cerr << exception.what() << std::endl;
    }
}

void buildComplexPlaylistScenario() {
    try {
        Playlist<double> playlist(30.0, "Complex Playlist.txt");

        FileSound<double> fileSound(30.0, "original.bin");
        Effect<double, AmplifyPolicy<double>> effect = EffectManager<double>::buildAmplifyEffect(fileSound, 2.0);

        playlist.replaceSound(2, effect);

        Sound<double>& result = static_cast<Sound<double>&>(playlist);
        std::cout << result << std::endl;
    } catch (std::exception& exception) {
        std::cerr << exception.what() << std::endl;
    }
}

int main() {
    // The file "original.bin" contains the values of:
    // 1    2   3   4   5   6   7   8   9   10

    // In order to be precise, I have commented the code
    // with the different scenarios to see the result
    // properly, because playlists are large from samples.

    // constructNormalizationEffectScenario();
    buildSimplePlaylistScenario();
    // buildAveragePlaylistScenario();
    // buildComplexPlaylistScenario();

    return 0;
}
