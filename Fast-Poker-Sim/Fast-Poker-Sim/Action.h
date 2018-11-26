#pragma once
#include <vector>

struct Action
{
	const enum ActionType { Bet, Call, Check, Fold, Raise, NoAction };

	ActionType type;
	uint32_t amount;
};
