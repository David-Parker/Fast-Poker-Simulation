#include <stdio.h>
#include <iostream>
#include "HoldEm.h"

int main(int argc, char** argv)
{
	HoldEm game(4);
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

	return 0;
}