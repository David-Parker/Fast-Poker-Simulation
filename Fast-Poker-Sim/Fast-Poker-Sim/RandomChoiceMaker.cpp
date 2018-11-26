#include "RandomChoiceMaker.h"

RandomChoiceMaker::RandomChoiceMaker()
{
}


RandomChoiceMaker::~RandomChoiceMaker()
{
}

void RandomChoiceMaker::MakeChoice(GameState& gameState, uint32_t chipCount, Action& result)
{
	// Actions: Check, Call, Raise, Fold, NoAction (only for end state)
	if (gameState.currentStreetState == GameState::StreetStates::End)
	{
		result.type = Action::ActionType::NoAction;
	}
	else if (gameState.currentBetState == GameState::BetStates::NoBets)
	{
		// Bet, Check, Fold
		result.type = Action::ActionType::Fold;
	}
	else if (gameState.currentBetState == GameState::BetStates::Bets)
	{
		// Call, Raise, Fold
		result.type = Action::ActionType::Fold;
	}
}
