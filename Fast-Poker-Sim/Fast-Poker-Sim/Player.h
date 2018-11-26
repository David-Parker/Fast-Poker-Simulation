#pragma once
#include <vector>
#include "Action.h"
#include "Card.h"
#include "GameState.h"
#include "IChoiceMaker.h"

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

	Player() : isActive(false), isPlaying(false), chips(0) {}
	~Player() {}

	inline void MakeChoice(GameState& gameState, uint32_t chipCount, Action& result)
	{
		assert(choiceMaker != nullptr);
		this->choiceMaker->MakeChoice(gameState, chipCount, result);
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
		delete this->choiceMaker;
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
	inline uint32_t PlaceBet(GameState& gameState, uint32_t amount)
	{
		uint32_t chips = this->chips;
		assert(this->chips > 0);
		assert(this->isActive == true);
		assert(this->isPlaying == true);
		assert(amount > 0);
		gameState.currentBetState = GameState::BetStates::Bets;
		uint32_t actualBet = amount > chips ? chips : amount;
		gameState.AddToPot(actualBet);
		this->chips -= actualBet;
		return actualBet;
	}
};