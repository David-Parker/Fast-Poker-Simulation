#pragma once
#include <vector>
#include "Card.h"

struct PlayerState
{
	Card* hand[2];
	uint32_t totalBet;
	bool isAllIn;
	char potNum;
	uint16_t handRank;

	PlayerState() : potNum(-1), handRank(0) {}

	inline void Clear()
	{
		hand[0] = nullptr;
		hand[1] = nullptr;
		totalBet = 0;
		isAllIn = false;
		potNum = -1;
		handRank = 0;
	}

	inline void ClearBet()
	{
		totalBet = 0;
	}
};