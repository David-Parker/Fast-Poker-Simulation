#include "RandomChoiceMaker.h"

RandomChoiceMaker::RandomChoiceMaker()
{
}


RandomChoiceMaker::~RandomChoiceMaker()
{
}

void RandomChoiceMaker::MakeChoice(GameState& gameState)
{
	// Actions: Check, Call, Raise, Fold

	if (gameState.currentTurnState == GameState::StreetStates::End)
	{
		return;
	}

	if (gameState.currentHandState == GameState::BetStates::NoBets)
	{
		// Bet, Call, Fold
	}

	if (gameState.currentHandState == GameState::BetStates::Bets)
	{

	}
}
