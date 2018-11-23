#include <stdio.h>
#include <iostream>
#include "HoldEm.h"

int main()
{
	HoldEm game(2);
	int gamesPlayed = 0;
	game.Start();

	// game loop
	while (game.Update(gamesPlayed))
	{
		if (gamesPlayed == 100)
		{
			break;
		}
	}

	std::cout << "Done.";

	getchar();
	return 0;
}