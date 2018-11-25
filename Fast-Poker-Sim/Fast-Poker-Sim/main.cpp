#include <stdio.h>
#include <iostream>
#include "HoldEm.h"

int main()
{
	HoldEm game(8);
	int gamesPlayed = 0;
	game.Start();

	// game loop
	while (game.Update(gamesPlayed))
	{
		//std::cout << gamesPlayed << std::endl;
		if (gamesPlayed == 1000)
		{
			break;
		}
	}

	std::cout << "Done.";

	getchar();
	return 0;
}