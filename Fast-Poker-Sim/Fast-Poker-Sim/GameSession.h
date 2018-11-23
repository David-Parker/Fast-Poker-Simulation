#pragma once
#include <vector>
#include <cassert>
#include <iterator>
#include "Card.h"

/* Stores the state of a current game session, and accepts actions from players. */
class GameSession
{
private:
	char turn;

public:
	GameSession();
	~GameSession();
	std::vector<Card> table;
	std::vector<Card> deck;
	bool complete;

	void NewSession();
	void NextTurn();

	inline void GetCards(std::vector<Card>& hand)
	{
		assert(deck.size() >= 2);
		// Inserts two cards from the deck to the hand passed out by reference.
		std::move(deck.end() - 2, deck.end(), std::back_inserter(hand));
	}
};

