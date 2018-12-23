#pragma once
#include <vector>

struct Action
{
	const enum ActionType { Bet, Call, Check, Fold, Raise, NoAction };

	static char* actions[6];

	ActionType type;
	uint32_t amount;
};
