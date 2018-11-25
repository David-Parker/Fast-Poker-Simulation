#pragma once
#include "GameState.h"

/* Choice making interface, allows us to inject new decision makers. */
class IChoiceMaker
{
public:
	virtual void MakeChoice(GameState& state) = 0;
};
