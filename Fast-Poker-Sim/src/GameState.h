#pragma once
#include <vector>
#include <cassert>
#include "Card.h"
#include "GameConfiguration.h"

/* Represents the publically visible state of the game to all the players. */
struct GameState
{
public:
	// Definitions of all the possible game states.
	const enum StreetStates { Deal, Flop, Turn, River, End };
	const enum BetStates { NoBets, Bets };

	char currentStreetState;
	char currentBetState;
	Card* table[5];
	char numCards;
	char numPlaying;
	char currentPot;
	uint32_t pots[MAX_PLAYERS];
	char playerBigBlind;
	char playerSmallBlind;
	uint32_t betAmount;

	GameState() : currentStreetState(StreetStates::Deal), currentBetState(BetStates::NoBets), table(), numPlaying(0), currentPot(0), playerBigBlind(0), playerSmallBlind(1), betAmount(0), pots() {}

	inline void ClearTable()
	{
		for (char i = 0; i < 5; ++i)
		{
			table[i] = nullptr;
		}

		numCards = 0;
	}

	inline void AddToPot(uint32_t amount)
	{
		assert(amount > 0);
		this->pots[this->currentPot] += amount;
	}

	inline void Reset()
	{
		this->currentStreetState = StreetStates::Deal;
		this->currentBetState = BetStates::NoBets;
		this->numPlaying = 0;
		this->currentPot = 0;
		this->playerBigBlind = 0;
		this->playerSmallBlind = 0;
		this->betAmount = 0;

		for (char i = 0; i < MAX_PLAYERS; ++i)
		{
			pots[i] = 0;
		}

		ClearTable();
	}
};
