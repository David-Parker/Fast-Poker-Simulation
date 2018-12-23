#include <algorithm>
#include <iterator>
#include <iostream>
#include "Action.h"
#include "GameSession.h"
#include "Logger.h"

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
	uint32_t smallBet = smallBlindPlayer->PlaceBet(this->gameState, SMALL_BLIND, this->playerStates[this->gameState.playerSmallBlind].isAllIn);
	uint32_t bigBet = bigBlindPlayer->PlaceBet(this->gameState, BIG_BLIND, this->playerStates[this->gameState.playerBigBlind].isAllIn);
	this->gameState.betAmount = BIG_BLIND;
	this->playerStates[this->gameState.playerSmallBlind].totalBet = smallBet;
	this->playerStates[this->gameState.playerBigBlind].totalBet = bigBet;
}

void GameSession::DistributePot()
{
	if (gameState.numPlaying > 1)
	{
		log("Went all the way.\n");
	}

	for (char i = 0; i < MAX_PLAYERS; ++i)
	{
		Player& p = players[i];

		if (p.isPlaying)
		{
			// find the player(s) with the best 5 card hand.
			p.chips += this->gameState.currentPot;
			log("Player %i won %d.\n", i, this->gameState.currentPot);
			break;
		}
	}
}

GameSession::GameSession() : deck(), complete(false), gameState(), gameStatePrevious(), players(), topOfDeck(51), rand(), playerStates()
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
	log("New Session\n");
	this->complete = false;
	this->gameStatePrevious = this->gameState;
	this->players = players;

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
			log("Player %d has %d chips.\n", p.playerNum, p.chips);
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
	this->gameState.betAmount = 0;

	for (char i = 0; i < MAX_PLAYERS; ++i)
	{
		this->playerStates[i].ClearBet();
	}

	if (this->gameState.numPlaying == 1)
	{
		this->gameState.currentStreetState = GameState::StreetStates::End;
	}

	switch (this->gameState.currentStreetState)
	{
		// Post blinds and Deal cards to all players.
		case GameState::StreetStates::Deal:
			log("Posting blinds.\n");
			PostBlinds();
			log("Dealing.\n");
			DealCards();
			break;
		// Flop, move 3 cards from the deck to the table.
		case GameState::StreetStates::Flop:
			log("Flop.\n");
			MoveCardsFromDeck(3, this->gameState.table);
			this->gameState.numCards += 3;
			break;
		// Turn and river, move one card from the deck to the table.
		case GameState::StreetStates::Turn:
			log("Turn.\n");
			MoveCardsFromDeck(1, this->gameState.table);
			this->gameState.numCards += 1;
			break;
		case GameState::StreetStates::River:
			log("River.\n");
			MoveCardsFromDeck(1, this->gameState.table);
			this->gameState.numCards += 1;
			break;
		case GameState::StreetStates::End:
			log("Ending of play.\n");
			DistributePot();
			this->complete = true;
			break;
		default:
			assert(false);
	}
}

void GameSession::HandlePlayerChoices()
{
	assert(this->gameState.numPlaying >= 1);

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
				// All other players folded, end the game and distribute the winnings.
				if (this->gameState.numPlaying == 1)
				{
					this->gameState.currentStreetState = GameState::StreetStates::End;
				}

				Action action;
				p.MakeChoice(this->gameState, p.chips, action, playerStates[index]);
				uint32_t bet = 0;

				switch (this->gameState.currentStreetState)
				{
				case GameState::StreetStates::End:
					assert(action.type == Action::ActionType::NoAction);
					turnComplete = true;
					break;
				case GameState::StreetStates::Deal:
				case GameState::StreetStates::Flop:
				case GameState::StreetStates::Turn:
				case GameState::StreetStates::River:
					switch (this->gameState.currentBetState)
					{
						case GameState::BetStates::NoBets:
							assert(action.type == Action::ActionType::Bet || action.type == Action::ActionType::Check || action.type == Action::ActionType::Fold || action.type == Action::ActionType::NoAction);
							switch (action.type)
							{
								case Action::ActionType::Bet:
									turnComplete = false;
									bet = p.PlaceBet(gameState, action.amount, playerStates[index].isAllIn);
									playerStates[index].totalBet += bet;
									gameState.betAmount = bet;
									break;
								case Action::ActionType::Check:
									break;
								case Action::ActionType::Fold:
									p.Quit();
									this->gameState.numPlaying--;
									break;
								case Action::ActionType::NoAction:
									break;
							}
							break;
						case GameState::BetStates::Bets:
							assert(action.type == Action::ActionType::Call || action.type == Action::ActionType::Raise || action.type == Action::ActionType::Fold || action.type == Action::ActionType::NoAction);
							switch (action.type)
							{
								case Action::ActionType::Call:
									if (playerStates[index].totalBet < this->gameState.betAmount && playerStates[index].isAllIn == false)
									{
										bet = p.PlaceBet(gameState, this->gameState.betAmount - playerStates[index].totalBet, playerStates[index].isAllIn);
										playerStates[index].totalBet += bet;
									}
									break;
								case Action::ActionType::Raise:
									turnComplete = false;
									bet = p.PlaceBet(gameState, action.amount, playerStates[index].isAllIn);
									playerStates[index].totalBet += bet;
									assert(bet > 0);
									gameState.betAmount = bet;
									break;
								case Action::ActionType::Fold:
									p.Quit();
									this->gameState.numPlaying--;
									break;
								case Action::ActionType::NoAction:
									break;
							}
							break;
					}
				}
			}
		}
	}

	if (this->gameState.currentStreetState < GameState::StreetStates::End)
	{
		this->gameState.currentStreetState++;
	}

	this->gameState.currentBetState = GameState::BetStates::NoBets;
}
