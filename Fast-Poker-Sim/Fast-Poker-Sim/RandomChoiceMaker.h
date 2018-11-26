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

	void MakeChoice(GameState& gameState, uint32_t chipCount, Action& result) override;
};

