#pragma once
#include "GameState.h"
#include "Action.h"

/* Choice making interface, allows us to inject new decision makers. */
class IChoiceMaker
{
public:
	virtual void MakeChoice(GameState& state, uint32_t chipCount, Action& result) = 0;
};
