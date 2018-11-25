#pragma once
#include "Action.h"
#include "IChoiceMaker.h"
#include "GameState.h"

/* Makes random choices. */
class RandomChoiceMaker : public IChoiceMaker
{
public:
	RandomChoiceMaker();
	~RandomChoiceMaker();

	Action::ActionType MakeChoice(GameState& gameState) override;
};

