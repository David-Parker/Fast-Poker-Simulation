#include "RandomChoiceMaker.h"
#include "Logger.h"

FastRand RandomChoiceMaker::rand = FastRand();

RandomChoiceMaker::RandomChoiceMaker()
{
}


RandomChoiceMaker::~RandomChoiceMaker()
{
}

void RandomChoiceMaker::MakeChoice(const GameState& gameState, uint32_t chipCount, Action& result, const PlayerState& playerState)
{
	// Actions: Check, Call, Raise, Fold, NoAction (only for end state)
	if (gameState.currentStreetState == GameState::StreetStates::End || playerState.isAllIn == true || gameState.numPlaying == 1)
	{
		result.type = Action::ActionType::NoAction;
		return;
	}

	if (gameState.currentBetState == GameState::BetStates::NoBets)
	{
		// Bet, Check, Fold
		int prob = rand.Rand() % 3;

		switch (prob)
		{
			case 0:
				assert(chipCount > 0);
				result.type = Action::ActionType::Bet;
				result.amount = (rand.Rand() % chipCount) + 1;
				break;
			case 1:
				result.type = Action::ActionType::Check;
				break;
			case 2:
				result.type = Action::ActionType::Fold;
				break;
			default:
				assert(false);
				break;
		}
	}
	else if (gameState.currentBetState == GameState::BetStates::Bets)
	{
		// Player does not need to call, has already bet the maximum.
		if (gameState.betAmount == playerState.totalBet)
		{
			result.type = Action::ActionType::NoAction;
			return;
		}

		if (gameState.betAmount >= chipCount)
		{
			// Call, Fold
			int prob = rand.Rand() % 2;

			switch (prob)
			{
			case 0:
				assert(chipCount > 0);
				result.type = Action::ActionType::Call;
				break;
			case 1:
				result.type = Action::ActionType::Fold;
				break;
			default:
				assert(false);
				break;
			}
		}
		else
		{
			// Call, Raise, Fold
			int prob = rand.Rand() % 3;
			uint32_t maxRaise = 0;

			switch (prob)
			{
			case 0:
				assert(chipCount > 0);
				result.type = Action::ActionType::Call;
				break;
			case 1:
				assert(chipCount > 0 && chipCount > gameState.betAmount);
				result.type = Action::ActionType::Raise;
				maxRaise = chipCount - gameState.betAmount;
				result.amount = gameState.betAmount + (rand.Rand() % maxRaise);
				break;
			case 2:
				result.type = Action::ActionType::Fold;
				break;
			default:
				assert(false);
				break;
			}
		}
	}
}
