#pragma once
#include <vector>
#include <cassert>
#include <iterator>
#include "FastRand.h"
#include "GameConfiguration.h"
#include "GameState.h"
#include "Player.h"
#include "PlayerState.h"
#include "Card.h"

/* Stores the state of a current game session, and accepts actions from players. */
class GameSession
{
typedef Player(&Players)[MAX_PLAYERS];

private:
	Card deck[52];
	char topOfDeck;
	Player* players;
	PlayerState playerStates[MAX_PLAYERS];
	FastRand rand;

	inline void ShuffleDeck()
	{
		for (char i = 52 - 1; i > 0; --i)
		{
			char j = rand.Rand() % (i + 1);

			Card::Swap(&deck[i], &deck[j]);
		}
	}

	inline void DealCards()
	{
		for (char i = 0; i < MAX_PLAYERS; ++i)
		{
			Player& p = players[i];

			if (p.isPlaying)
			{
				GetCards(playerStates[i].hand);
				Card* c1 = playerStates[i].hand[0];
				Card* c2 = playerStates[i].hand[1];
				log("Player %d was dealt %s of %s and %s of %s.\n", i, Card::ranks[c1->rank - 1], Card::suits[c1->suit - 1], Card::ranks[c2->rank - 1], Card::suits[c2->suit - 1]);
			}
		}
	}

	inline void MoveCardsFromDeck(char numCards, Card** destination)
	{
		assert(numCards <= 52);
		assert(topOfDeck > 0);
		assert(topOfDeck >= numCards);

		for (char i = 0; i < numCards; ++i)
		{
			destination[i] = (&deck[topOfDeck--]);
		}
	}

	void PostBlinds();
	void DistributePot();

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

