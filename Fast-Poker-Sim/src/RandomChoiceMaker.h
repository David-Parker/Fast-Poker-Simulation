#pragma once
#include "Action.h"
#include "IChoiceMaker.h"
#include "GameState.h"
#include "FastRand.h"

/* Makes random choices. */
class RandomChoiceMaker : public IChoiceMaker
{
private:
	static FastRand rand;

public:
	RandomChoiceMaker();
	~RandomChoiceMaker();

	void MakeChoice(const GameState& gameState, uint32_t chipCount, Action& result, const PlayerState& playerState) override;
};

