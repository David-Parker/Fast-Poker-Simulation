#include "GameSession.h"
#include <algorithm>
#include <iterator>

GameSession::GameSession() : table(), deck(52), turn(0), complete(false)
{
	NewSession();
}

GameSession::~GameSession()
{
}

void GameSession::NewSession()
{
	table.clear();

	char index = 0;
	for (char i = 2; i <= 14; ++i)
	{
		for (char j = 1; j <= 4; ++j)
		{
			deck[index++].Assign(i, j);
		}
	}

	std::random_shuffle(deck.begin(), deck.end());
}

void GameSession::NextTurn()
{
	assert(this->turn < 4);

	// Flop, move 3 cards from the deck to the table.
	if (this->turn == 0)
	{
		std::move(deck.end() - 3, deck.end(), std::back_inserter(table));
	}
	// Turn and river, move one card from the deck to the table.
	else if (this->turn == 1 || this->turn == 2)
	{
		std::move(deck.end() - 1, deck.end(), std::back_inserter(table));
	}
	else if (this->turn == 3)
	{
		this->complete = true;
	}

	this->turn++;
}