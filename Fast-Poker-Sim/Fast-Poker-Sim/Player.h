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
	Card* hand[2];

	Player() : isActive(false), isPlaying(false), chips(0), hand() {}
	~Player() {}

	inline Action::ActionType MakeChoice(GameState& gameState)
	{
		assert(choiceMaker != nullptr);
		return this->choiceMaker->MakeChoice(gameState);
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

	inline void ClearHand()
	{
		hand[0] = nullptr;
		hand[1] = nullptr;
	}

	inline void Quit()
	{
		assert(this->isActive == true);
		this->isPlaying = false;
	}

	inline void PlaceBet(GameState& gameState, uint32_t amount)
	{
		uint32_t chips = this->chips;
		assert(this->chips > 0);
		assert(this->isActive == true);
		assert(this->isPlaying == true);
		gameState.currentHandState = GameState::BetStates::Bets;
		uint32_t actualBet = amount > chips ? chips : amount;
		gameState.currentPot += actualBet;
		this->chips -= actualBet;
	}
};