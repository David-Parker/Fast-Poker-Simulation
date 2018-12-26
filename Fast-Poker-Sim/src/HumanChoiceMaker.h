#pragma once
#include "IChoiceMaker.h"
#include "Action.h"
#include "IChoiceMaker.h"
#include "GameState.h"
#include "FastRand.h"
#include "Player.h"

class HumanChoiceMaker : public IChoiceMaker
{
private:
	static FastRand rand;

public:
	HumanChoiceMaker();
	~HumanChoiceMaker();

	void MakeChoice(const GameState& gameState, uint32_t chipCount, Action& result, const PlayerState& playerState, const Player& player) override;
};

