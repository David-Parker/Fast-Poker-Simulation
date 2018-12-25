#pragma once
#include "GameState.h"
#include "PlayerState.h"
#include "Action.h"

class Player;

/* Choice making interface, allows us to inject new decision makers. */
class IChoiceMaker
{
public:
	virtual void MakeChoice(const GameState& state, uint32_t chipCount, Action& result, const PlayerState& playerState, const Player& player) = 0;
};
