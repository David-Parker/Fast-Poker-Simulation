#include "RandomChoiceMaker.h"

RandomChoiceMaker::RandomChoiceMaker()
{
}


RandomChoiceMaker::~RandomChoiceMaker()
{
}

Action::ActionType RandomChoiceMaker::MakeChoice(GameState& gameState)
{
	// Actions: Check, Call, Raise, Fold, NoAction (only for end state)
	if (gameState.currentTurnState == GameState::StreetStates::End)
	{
		return Action::ActionType::NoAction;
	}
	else if (gameState.currentHandState == GameState::BetStates::NoBets)
	{
		// Bet, Check, Fold
		return Action::ActionType::Fold;
	}
	else if (gameState.currentHandState == GameState::BetStates::Bets)
	{
		// Call, Raise, Fold
		return Action::ActionType::Fold;
	}
}
