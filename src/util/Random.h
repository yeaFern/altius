#pragma once

#include <cstdlib>
#include <cstdint>

// Stinky rand() based RNG.
class Random
{
public:
    static void SetSeed(uint32_t seed);

    // Generates a random float in the range of [0, 1].
    static float Float();

    // Generates a random number in the range [min, max). 
    static uint32_t Range(uint32_t min, uint32_t max);

	static float Noise(uint32_t seed);
};
