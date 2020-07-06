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

float Random::Noise(uint32_t seed)
{
	seed = (seed << 13) ^ seed;
	seed = seed * (seed * seed * 15731 + 789221) + 1376312589;
	return float(seed & 0x7fffffffU) / float(0x7fffffff);
}
