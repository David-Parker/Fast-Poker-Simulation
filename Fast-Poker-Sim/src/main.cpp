#include <stdio.h>
#include <iostream>
#include "HoldEm.h"

int main(int argc, char** argv)
{
	HoldEm game(4);
	int gamesPlayed = 0;

	for (int i = 0; i < 1; ++i)
	{
		game.NewGame();
		game.Start();

		while (game.Update(gamesPlayed))
		{
		}
	}

	return 0;
}