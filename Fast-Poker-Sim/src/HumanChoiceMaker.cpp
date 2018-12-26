#include <iostream>
#include <string>
#include "HumanChoiceMaker.h"

FastRand HumanChoiceMaker::rand = FastRand();

HumanChoiceMaker::HumanChoiceMaker()
{
}

HumanChoiceMaker::~HumanChoiceMaker()
{
}

void HumanChoiceMaker::MakeChoice(const GameState& gameState, uint32_t chipCount, Action& result, const PlayerState& playerState, const Player& player)
{
	// Actions: Check, Call, Raise, Fold, NoAction (only for end state)
	if (gameState.currentStreetState == GameState::StreetStates::End || playerState.isAllIn == true || gameState.numPlaying == 1)
	{
		result.type = Action::ActionType::NoAction;
		return;
	}

	if (gameState.currentBetState == GameState::BetStates::NoBets)
	{
		std::cout << "Please make a choice: bet, check, or fold." << std::endl;

		while (1)
		{
			std::string response;

			std::cin >> response;

			// Bet, Check, Fold
			if (response == "bet")
			{
				std::cout << "Please enter a bet amount. Chips available: " << chipCount <<  std::endl;

				assert(chipCount > 0);
				uint32_t bet;
				std::cin >> bet;
				result.type = Action::ActionType::Bet;

				if (bet > chipCount)
				{
					std::cout << "Invalid bet amount, chips remaining: " << chipCount << std::endl;
					continue;
				}

				result.amount = bet;
				break;
			}
			else if (response == "check")
			{
				result.type = Action::ActionType::Check;
				break;
			}
			else if (response == "fold")
			{
				result.type = Action::ActionType::Fold;
				break;
			}
			else
			{
				std::cout << "Unknown input, valid actions are: bet, check, fold." << std::endl;
				continue;
			}
		}
	}
	else if (gameState.currentBetState == GameState::BetStates::Bets)
	{
		// Player does not need to call, has already bet the maximum. Never call yourself.
		if (gameState.betAmount == playerState.totalBet || player.playerNum == gameState.lastBet)
		{
			result.type = Action::ActionType::NoAction;
			return;
		}

		if (gameState.betAmount >= chipCount)
		{
			std::cout << "Please make a choice: call, or fold." << std::endl;

			while (1)
			{
				std::string response;

				std::cin >> response;

				// Call, Fold
				if (response == "call")
				{
					assert(chipCount > 0);
					result.type = Action::ActionType::Call;
					break;
				}
				else if (response == "fold")
				{
					result.type = Action::ActionType::Fold;
					break;
				}
				else
				{
					std::cout << "Unknown input, valid actions are: call, fold." << std::endl;
					continue;
				}
			}
		}
		else
		{
			std::cout << "Please make a choice: call, raise, or fold." << std::endl;

			while (1)
			{
				std::string response;

				std::cin >> response;

				// Call, Raise, Fold
				if (response == "call")
				{
					assert(chipCount > 0);
					result.type = Action::ActionType::Call;
					break;
				}
				else if (response == "raise")
				{
					assert(chipCount > 0 && chipCount > gameState.betAmount);
					uint32_t raise;
					uint32_t maxRaise = chipCount - gameState.betAmount;
					std::cin >> raise;
					result.type = Action::ActionType::Raise;

					if (raise > maxRaise)
					{
						std::cout << "Invalid bet amount, max raise is: " << maxRaise << std::endl;
						continue;
					}

					result.amount = gameState.betAmount + raise;
					break;
				}
				else if (response == "fold")
				{
					result.type = Action::ActionType::Fold;
					break;
				}
				else
				{
					std::cout << "Unknown input, valid actions are: call, raise, fold." << std::endl;
					continue;
				}
			}
		}
	}
}
