#include <algorithm>
#include <iterator>
#include <iostream>
#include "Action.h"
#include "GameSession.h"

unsigned int GameSession::g_seed = 0;

void GameSession::PostBlinds()
{
	assert(this->gameState.numPlaying >= 2);

	char lastBigBlindPlayer = this->gameStatePrevious.playerBigBlind;
	char lastSmallBlindPlayer = this->gameStatePrevious.playerSmallBlind;
	Player* smallBlindPlayer = nullptr;
	Player* bigBlindPlayer = nullptr;

	char startIndex = lastBigBlindPlayer + 1;
	for (char i = startIndex; i < startIndex + MAX_PLAYERS; ++i)
	{
		char index = i % MAX_PLAYERS;
		Player& p = players[index];

		if (p.isPlaying)
		{
			if (bigBlindPlayer == nullptr)
			{
				bigBlindPlayer = &p;
				this->gameState.playerBigBlind = index;
			}
			else if (smallBlindPlayer == nullptr)
			{
				smallBlindPlayer = &p;
				this->gameState.playerSmallBlind = index;
			}
		}
	}

	assert(bigBlindPlayer != nullptr);
	assert(smallBlindPlayer != nullptr);
	assert(bigBlindPlayer != smallBlindPlayer);
	assert(bigBlindPlayer->chips > 0);
	assert(smallBlindPlayer->chips > 0);

	// Make big and small blind players bet.
	smallBlindPlayer->PlaceBet(this->gameState, SMALL_BLIND);
	bigBlindPlayer->PlaceBet(this->gameState, BIG_BLIND);
}

GameSession::GameSession() : deck(), complete(false), gameState(), gameStatePrevious(), players(), topOfDeck(51)
{
	char index = 0;
	for (char i = 2; i <= 14; ++i)
	{
		for (char j = 1; j <= 4; ++j)
		{
			deck[index++].Assign(i, j);
		}
	}
}

GameSession::~GameSession()
{
}

void GameSession::NewSession(Players players)
{
	this->complete = false;
	this->gameStatePrevious = this->gameState;
	this->players = players;

	this->gameState.Reset();

	this->topOfDeck = 51;
	ShuffleDeck();

	char numPlaying = 0;
	for (char i = 0; i < MAX_PLAYERS; ++i)
	{
		Player& p = players[i];

		if (p.isPlaying)
		{
			numPlaying++;
			p.ClearHand();
		}
	}

	this->gameState.numPlaying = numPlaying;
}

void GameSession::NextTurn()
{
	// Post blinds and Deal cards to all players.
	if (this->gameState.currentTurnState == GameState::StreetStates::Deal)
	{
		PostBlinds();

		for (char i = 0; i < MAX_PLAYERS; ++i)
		{
			Player& p = players[i];

			if (p.isPlaying)
			{
				GetCards(p.hand);
			}
		}
	}
	// Flop, move 3 cards from the deck to the table.
	else if (this->gameState.currentTurnState == GameState::StreetStates::Flop)
	{
		MoveCardsFromDeck(3, this->gameState.table);
	}
	// Turn and river, move one card from the deck to the table.
	else if (this->gameState.currentTurnState == GameState::StreetStates::Turn || this->gameState.currentTurnState == GameState::StreetStates::River)
	{
		MoveCardsFromDeck(1, this->gameState.table);
	}
	else if (this->gameState.currentTurnState == GameState::StreetStates::End)
	{
		this->complete = true;
		return;
	}
	else
	{
		assert(false);
	}

	this->gameState.currentTurnState++;
}

void GameSession::HandlePlayerChoices()
{
	for (char i = 0; i < MAX_PLAYERS; ++i)
	{
		Player& p = players[i];

		if (p.isPlaying)
		{
			Action::ActionType action = p.MakeChoice(this->gameState);

			if (this->gameState.currentTurnState == GameState::StreetStates::End)
			{
				assert(action == Action::ActionType::NoAction);
			}
			else if (this->gameState.currentHandState == GameState::BetStates::NoBets)
			{
				assert(action == Action::ActionType::Bet || action == Action::ActionType::Check || action == Action::ActionType::Fold);
			}
			else if (this->gameState.currentHandState == GameState::BetStates::Bets)
			{
				assert(action == Action::ActionType::Call || action == Action::ActionType::Raise || action == Action::ActionType::Fold);
			}
		}
	}
}
