#include <stdio.h>
#include <iostream>
#include <ctime>
#include "HoldEm.h"

int main(int argc, char** argv)
{
	HoldEm game(4);
	uint64_t gamesPlayed = 0;
	double duration;
	std::clock_t start = std::clock();

	for (int i = 0; i < 500000; ++i)
	{
		game.NewGame();
		game.Start();

		while (game.Update(gamesPlayed))
		{
		}
	}

	duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;

	std::cout << "Simulated " << gamesPlayed << " games in " << duration << " seconds.\n";

	getchar();

	return 0;
}