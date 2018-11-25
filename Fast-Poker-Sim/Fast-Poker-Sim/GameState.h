#pragma once
#include <vector>
#include "Card.h"

/* Represents the publically visible state of the game to all the players. */
struct GameState
{
public:
	// Definitions of all the possible game states.
	const enum StreetStates { Deal, Flop, Turn, River, End };
	const enum BetStates { NoBets, Bets };

	char currentTurnState;
	char currentHandState;
	std::vector<Card> table;
	char numPlaying;
	uint32_t currentPot;
	char playerBigBlind;
	char playerSmallBlind;

	GameState() : currentTurnState(StreetStates::Deal), currentHandState(BetStates::NoBets), table(), numPlaying(0), currentPot(0), playerBigBlind(0), playerSmallBlind(1) {}

	inline void Reset()
	{
		this->currentTurnState = StreetStates::Deal;
		this->currentHandState = BetStates::NoBets;
		this->table.clear();
		this->numPlaying = 0;
		this->currentPot = 0;
		this->playerBigBlind = 0;
		this->playerSmallBlind = 0;
	}
};
