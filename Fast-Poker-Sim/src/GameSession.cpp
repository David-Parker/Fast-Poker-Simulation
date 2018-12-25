#include <algorithm>
#include <iterator>
#include <iostream>
#include "Action.h"
#include "GameSession.h"
#include "Logger.h"
#include "SevenEval.h"

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
	smallBlindPlayer->PlaceBet(this->gameState, SMALL_BLIND, this->playerStates[this->gameState.playerSmallBlind]);
	bigBlindPlayer->PlaceBet(this->gameState, BIG_BLIND, this->playerStates[this->gameState.playerBigBlind]);
}

void GameSession::DistributePot()
{
	assert(gameState.numPlaying > 0);

	// Calculate the hand ranks for each player
	for (char i = 0; i < MAX_PLAYERS; ++i)
	{
		Player& p = players[i];

		if (p.isPlaying)
		{
			// The game did not complete, one winner who did not fold.
			if (gameState.numPlaying == 1)
			{
				playerStates[i].handRank = 1;
			}
			else
			{
				playerStates[i].handRank = SevenEval::GetRank(this->gameState.table[0]->val, this->gameState.table[1]->val, this->gameState.table[2]->val,
					this->gameState.table[3]->val, this->gameState.table[4]->val, this->playerStates[i].hand[0]->val, this->playerStates[i].hand[1]->val);
			}
		}
		else
		{
			playerStates[i].handRank = 0;
		}
	}

	// Calculate the winner for each pot.
	for (char i = 0; i < MAX_PLAYERS; ++i)
	{
		if (this->gameState.pots[i] > 0)
		{
			uint16_t maxValue = 0;
			char numWinners = 0;

			// Caculate the max
			for (char j = 0; j < MAX_PLAYERS; ++j)
			{
				PlayerState& state = playerStates[j];

				// Player is in this pot
				if (state.potNum >= i)
				{
					if (state.handRank > maxValue)
					{
						numWinners = 1;
						maxValue = state.handRank;
					}
					else if (state.handRank == maxValue)
					{
						numWinners++;
					}
				}
			}
			assert(numWinners > 0);

			uint32_t extra = this->gameState.pots[i] % numWinners;
			uint32_t split = this->gameState.pots[i] / numWinners;

			for (char j = 0; j < MAX_PLAYERS; ++j)
			{
				PlayerState& state = playerStates[j];

				// Player is in this pot
				if (state.potNum >= i)
				{
					if (state.handRank == maxValue)
					{
						uint32_t winnings = split + extra;
						players[j].chips += winnings;
						log("Player %d won %d from pot %d.\n", players[j].playerNum, winnings, i);
						extra = 0;
					}
				}
			}
		}
	}
}

GameSession::GameSession() : deck(), complete(false), gameState(), gameStatePrevious(), players(), topOfDeck(51), rand(), playerStates()
{
	char index = 0;
	for (char i = 0; i < 52; ++i)
	{
		deck[index++].Assign(i);
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
	uint32_t totalChips = 0;
	for (char i = 0; i < MAX_PLAYERS; ++i)
	{
		playerStates[i].Clear();

		Player& p = players[i];

		if (p.isActive)
		{
			p.Play();
			log("Player %d has %d chips.\n", p.playerNum, p.chips);
			totalChips += p.chips;
		}

		if (p.isPlaying)
		{
			numPlaying++;
		}
	}

	log("Total chips in play: %d.\n", totalChips);

	this->gameState.numPlaying = numPlaying;
}

void GameSession::NextTurn()
{
	this->gameState.betAmount = 0;

	for (char i = 0; i < MAX_PLAYERS; ++i)
	{
		assert(playerStates[i].totalBet == 0);
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
			MoveCardsFromDeck(3, &this->gameState.table[0]);
			log("Table flopped %s, %s, and %s.\n", Card::cards[this->gameState.table[0]->val], Card::cards[this->gameState.table[1]->val], Card::cards[this->gameState.table[2]->val]);
			this->gameState.numCards += 3;
			break;
		// Turn and river, move one card from the deck to the table.
		case GameState::StreetStates::Turn:
			log("Turn.\n");
			MoveCardsFromDeck(1, &this->gameState.table[3]);
			log("Table turned %s.\n", Card::cards[this->gameState.table[3]->val]);
			this->gameState.numCards += 1;
			break;
		case GameState::StreetStates::River:
			log("River.\n");
			MoveCardsFromDeck(1, &this->gameState.table[4]);
			log("Table rivered %s.\n", Card::cards[this->gameState.table[4]->val]);
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
				p.MakeChoice(this->gameState, p.chips, action, playerStates[index], players[index]);

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
									p.PlaceBet(gameState, action.amount, playerStates[index]);
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
									p.Call(gameState, this->gameState.betAmount - playerStates[index].totalBet, playerStates[index]);
									break;
								case Action::ActionType::Raise:
									turnComplete = false;
									p.PlaceBet(gameState, action.amount, playerStates[index]);
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

	// Calculate how the bets will be distributed among the various pots and side pots.
	if (gameState.betAmount > 0)
	{
		for (char i = 0; i < MAX_PLAYERS; ++i)
		{
			PlayerState& state = playerStates[i];

			if (state.totalBet > 0)
			{
				log("Player %d had a total bet of %d.\n", i, state.totalBet);
			}
		}

		for (char i = 0; i < MAX_PLAYERS; ++i)
		{
			// Find the minimum value among all the player's bets
			uint32_t min = UINT32_MAX;
			char minPlayer = -1;
			for (char j = 0; j < MAX_PLAYERS; ++j)
			{
				uint32_t bet = playerStates[j].totalBet;

				if (bet > 0 && bet < min)
				{
					min = bet;
					minPlayer = j;
				}
			}

			// All of the outstanding bets are 0, we are done.
			if (min == UINT32_MAX)
			{
				break;
			}

			// Subtract the minumum bet from all bets and add that to the current pot, if the min betting player went all in,
			// we create a new side pot by advancing the current pot index.
			for (char i = 0; i < MAX_PLAYERS; ++i)
			{
				PlayerState& state = playerStates[i];

				if (state.totalBet > 0)
				{
					state.potNum++;
					this->gameState.AddToPot(min);
					state.totalBet -= min;
				}
			}

			assert(minPlayer > -1);

			if (this->playerStates[minPlayer].isAllIn)
			{
				this->gameState.currentPot++;
			}
		}
	}

	if (this->gameState.currentStreetState < GameState::StreetStates::End)
	{
		this->gameState.currentStreetState++;
	}

	this->gameState.currentBetState = GameState::BetStates::NoBets;
}
