#pragma once
#include "IChoiceMaker.h"
#include "GameState.h"

/* Makes random choices. */
class RandomChoiceMaker : public IChoiceMaker
{
public:
	RandomChoiceMaker();
	~RandomChoiceMaker();

	void MakeChoice(GameState& gameState) override;
};

