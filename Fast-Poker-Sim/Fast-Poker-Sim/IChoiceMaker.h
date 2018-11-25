#pragma once
#include "GameState.h"
#include "Action.h"

/* Choice making interface, allows us to inject new decision makers. */
class IChoiceMaker
{
public:
	virtual Action::ActionType MakeChoice(GameState& state) = 0;
};
