#pragma once
#include <vector>
#include "Card.h"

struct PlayerState
{
	Card* hand[2];
	uint32_t totalBet;
	bool isAllIn;

	inline void Clear()
	{
		hand[0] = nullptr;
		hand[1] = nullptr;
		totalBet = 0;
		isAllIn = false;
	}

	inline void ClearBet()
	{
		totalBet = 0;
	}
};