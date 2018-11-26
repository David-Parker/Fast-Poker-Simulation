#include <algorithm>
#include <iterator>
#include <iostream>
#include "Action.h"
#include "GameSession.h"

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
	uint32_t smallBet = smallBlindPlayer->PlaceBet(this->gameState, SMALL_BLIND);
	uint32_t bigBet = bigBlindPlayer->PlaceBet(this->gameState, BIG_BLIND);
	this->betAmount = BIG_BLIND;
	this->playerStates[this->gameState.playerSmallBlind].totalBet = smallBet;
	this->playerStates[this->gameState.playerBigBlind].totalBet = bigBet;
	this->playerStates[this->gameState.playerSmallBlind].isAllIn = smallBlindPlayer->chips == 0;
	this->playerStates[this->gameState.playerBigBlind].isAllIn = bigBlindPlayer->chips == 0;
}

void GameSession::DistributePot()
{
}

GameSession::GameSession() : deck(), complete(false), gameState(), gameStatePrevious(), players(), topOfDeck(51), rand(), betAmount(0), playerStates()
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
	this->betAmount = 0;

	this->gameState.Reset();

	this->topOfDeck = 51;
	ShuffleDeck();

	char numPlaying = 0;
	for (char i = 0; i < MAX_PLAYERS; ++i)
	{
		playerStates[i].Clear();

		Player& p = players[i];

		if (p.isActive)
		{
			p.Play();
		}

		if (p.isPlaying)
		{
			numPlaying++;
		}
	}

	this->gameState.numPlaying = numPlaying;
}

void GameSession::NextTurn()
{
	switch (this->gameState.currentStreetState)
	{
		// Post blinds and Deal cards to all players.
		case GameState::StreetStates::Deal:
			PostBlinds();
			DealCards();
			break;
		// Flop, move 3 cards from the deck to the table.
		case GameState::StreetStates::Flop:
			MoveCardsFromDeck(3, this->gameState.table);
			this->gameState.numCards += 3;
			break;
		// Turn and river, move one card from the deck to the table.
		case GameState::StreetStates::Turn:
		case GameState::StreetStates::River:
			MoveCardsFromDeck(1, this->gameState.table);
			this->gameState.numCards += 1;
		case GameState::StreetStates::End:
			this->complete = true;
			break;
		default:
			assert(false);
	}
}

void GameSession::HandlePlayerChoices()
{
	assert(this->gameState.numPlaying >= 2);

	char startIndex = this->gameState.playerSmallBlind;
	bool turnComplete = false;

	while (!turnComplete)
	{
		// After we loop through each player, assume the turn is complete unless otherwise specified.
		turnComplete = true;

		// Loop around the table asking for player choices, this is a critical loop, must be performant.
		for (char i = startIndex; i < startIndex + MAX_PLAYERS; ++i)
		{
			char index = i % MAX_PLAYERS;

			Player& p = players[index];

			if (p.isPlaying)
			{
				Action action;
				p.MakeChoice(this->gameState, p.chips, action);
				uint32_t bet = 0;

				switch (this->gameState.currentStreetState)
				{
				case GameState::StreetStates::End:
					assert(action.type == Action::ActionType::NoAction);
					DistributePot();
					turnComplete = true;
					break;
				case GameState::StreetStates::Deal:
				case GameState::StreetStates::Flop:
				case GameState::StreetStates::Turn:
				case GameState::StreetStates::River:
					switch (this->gameState.currentBetState)
					{
						case GameState::BetStates::NoBets:
							assert(action.type == Action::ActionType::Bet || action.type == Action::ActionType::Check || action.type == Action::ActionType::Fold);
							switch (action.type)
							{
								case Action::ActionType::Bet:
									turnComplete = false;
									bet = p.PlaceBet(gameState, action.amount);
									playerStates[index].totalBet += bet;
									this->betAmount += bet;
									break;
								case Action::ActionType::Check:
									break;
								case Action::ActionType::Fold:
									p.Quit();
									break;
							}
							break;
						case GameState::BetStates::Bets:
							assert(action.type == Action::ActionType::Call || action.type == Action::ActionType::Raise || action.type == Action::ActionType::Fold);
							switch (action.type)
							{
								case Action::ActionType::Call:
									if (playerStates[index].totalBet < this->betAmount && playerStates[index].isAllIn == false)
									{
										bet = p.PlaceBet(gameState, betAmount);
										playerStates[index].totalBet += bet;
									}
									break;
								case Action::ActionType::Raise:
									turnComplete = false;
									bet = p.PlaceBet(gameState, action.amount);
									playerStates[index].totalBet += bet;
									this->betAmount += bet;
									break;
								case Action::ActionType::Fold:
									p.Quit();
									break;
							}
							break;
					}
				}

				playerStates[index].isAllIn = p.chips == 0;
			}
		}
	}

	this->gameState.currentStreetState++;
	this->gameState.currentBetState = GameState::BetStates::NoBets;
}
