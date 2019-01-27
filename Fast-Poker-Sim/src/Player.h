#pragma once
#include <vector>
#include "Action.h"
#include "Card.h"
#include "GameState.h"
#include "PlayerState.h"
#include "IChoiceMaker.h"
#include "Logger.h"

/* Represents a player in Texas Hold 'Em */
class Player
{
private:
	IChoiceMaker* choiceMaker;

public:
	bool isActive;
	bool isPlaying;
	char playerNum;
	uint32_t chips;

	Player() : isActive(false), isPlaying(false), chips(0), choiceMaker(nullptr) {}
	~Player() {}

	inline void MakeChoice(const GameState& gameState, uint32_t chipCount, Action& result, const PlayerState& playerState, const Player& player)
	{
		assert(choiceMaker != nullptr);
		this->choiceMaker->MakeChoice(gameState, chipCount, result, playerState, player);
		log("Player %d %s.\n", this->playerNum, Action::actions[result.type]);
	}

	inline void Activate(char playerNum, uint32_t chips, IChoiceMaker* choiceMaker)
	{
		this->isActive = true;
		this->isPlaying = false;
		this->playerNum = playerNum;
		this->chips = chips;
		this->choiceMaker = choiceMaker;
	}

	inline void Deactivate()
	{
		this->isActive = false;
		this->isPlaying = false;

		if (this->choiceMaker != nullptr)
		{
			delete this->choiceMaker;
			this->choiceMaker = nullptr;
		}
	}

	inline void Play()
	{
		assert(this->isActive == true);
		assert(this->chips > 0);
		this->isPlaying = true;
	}

	inline void Quit()
	{
		assert(this->isActive == true);
		this->isPlaying = false;
	}

	// Returns the size of the actual bet.
	inline void PlaceBet(GameState& gameState, uint32_t amount, PlayerState& playerState)
	{
		assert(this->chips > 0);
		assert(this->isActive == true);
		assert(this->isPlaying == true);
		assert(amount > 0);
		gameState.currentBetState = GameState::BetStates::Bets;
		uint32_t previousBet = amount - gameState.betAmount;
		uint32_t actualBet = amount > this->chips ? this->chips : amount;
		this->chips -= actualBet - playerState.totalBet;
		gameState.lastBet = this->playerNum;
		gameState.previousRaise = previousBet;
		gameState.betAmount = actualBet > gameState.betAmount ? actualBet : gameState.betAmount;
		playerState.totalBet = actualBet;

		log("Player %d placed bet %d.\n", this->playerNum, actualBet);

		if (this->chips == 0)
		{
			// must consider if an all-in is a CALL or a RAISE in some situations
			log("Player %d is all in.\n", this->playerNum);
			playerState.isAllIn = true;
		}
	}

	inline void Call(GameState& gameState, uint32_t amount, PlayerState& playerState)
	{
		assert(this->chips > 0);
		assert(this->isActive == true);
		assert(this->isPlaying == true);
		assert(amount > 0);
		uint32_t actualCall = amount > this->chips ? this->chips : amount;
		this->chips -= actualCall;
		playerState.totalBet += actualCall;

		log("Player %d called %d.\n", this->playerNum, actualCall);

		if (this->chips == 0)
		{
			log("Player %d is all in.\n", this->playerNum);
			playerState.isAllIn = true;
		}
	}
};