#pragma once
#include <vector>
#include <cassert>
#include <iterator>
#include "GameConfiguration.h"
#include "GameState.h"
#include "Player.h"
#include "Card.h"

/* Stores the state of a current game session, and accepts actions from players. */
class GameSession
{
typedef Player(&Players)[MAX_PLAYERS];

private:
	static unsigned int g_seed;
	Card deck[52];
	char topOfDeck;
	Player* players;
	void PostBlinds();

	inline void fast_srand(int seed) {
		g_seed = seed;
	}

	inline int fast_rand() {
		g_seed = (214013 * g_seed + 2531011);
		return (g_seed >> 16) & 0x7FFF;
	}

	inline void ShuffleDeck()
	{
		for (char i = 52 - 1; i > 0; --i)
		{
			char j = fast_rand() % (i + 1);

			Card::Swap(&deck[i], &deck[j]);
		}
	}

	inline void MoveCardsFromDeck(char numCards, Card** destination)
	{
		assert(numCards <= 52);

		for (char i = 0; i < numCards; ++i)
		{
			destination[i] = (&deck[topOfDeck--]);
		}
	}

public:
	GameSession();
	~GameSession();
	GameState gameStatePrevious;
	GameState gameState;
	bool complete;

	void NewSession(Players players);
	void NextTurn();
	void HandlePlayerChoices();

	inline void GetCards(Card** hand)
	{
		assert(topOfDeck >= 1);
		// Inserts two cards from the deck to the hand passed out by reference.
		MoveCardsFromDeck(2, hand);
	}
};

