#include "Random.h"

void Random::SetSeed(uint32_t seed)
{
    srand(seed);
}

float Random::Float()
{
    return float(rand()) / float(RAND_MAX);
}

uint32_t Random::Range(uint32_t min, uint32_t max)
{
    return rand() % (max - min) + min;
}
