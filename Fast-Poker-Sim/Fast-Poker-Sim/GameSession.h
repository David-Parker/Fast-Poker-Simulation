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
	Player* players;
	void PostBlinds();

	inline void MoveCardsFromDeck(char numCards, std::vector<Card>& destination)
	{
		assert(numCards <= 52);

		std::move(deck.end() - numCards, deck.end(), std::back_inserter(destination));
		deck._Pop_back_n(numCards);
	}

public:
	GameSession();
	~GameSession();
	GameState gameStatePrevious;
	GameState gameState;
	std::vector<Card> deck;
	bool complete;

	void NewSession(Players players);
	void NextTurn();
	void HandlePlayerChoices();

	inline void GetCards(std::vector<Card>& hand)
	{
		assert(deck.size() >= 2);
		// Inserts two cards from the deck to the hand passed out by reference.
		MoveCardsFromDeck(2, hand);
	}
};

