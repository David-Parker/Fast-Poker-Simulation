#pragma once
#include <time.h>

class FastRand
{
private:
	unsigned int g_seed;
public:
	inline void Seed(int seed) {
		g_seed = seed;
	}

	inline int Rand() {
		g_seed = (214013 * g_seed + 2531011);
		return (g_seed >> 16) & 0x7FFF;
	}

	FastRand()
	{
		Seed(time(NULL));
	}
};
