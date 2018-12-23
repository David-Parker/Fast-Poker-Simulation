#include <cassert>
#include "HoldEm.h"
#include "RandomChoiceMaker.h"
#include "Logger.h"

void HoldEm::RemoveLosingPlayers()
{
	for (char i = 0; i < MAX_PLAYERS; ++i)
	{
		Player& p = players[i];

		if (p.isActive == true)
		{
			assert(p.chips >= 0);

			if (p.chips == 0)
			{
				log("Player %d was eliminated.\n", i);
				RemovePlayer(i);
			}
		}
	}
}

HoldEm::HoldEm(char numPlayers) : numPlayers(), isPlaying(false), players(), session()
{
	assert(numPlayers >= 2);
	assert(numPlayers <= MAX_PLAYERS);

	for (char i = 0; i < numPlayers; ++i)
	{
		AddPlayer();
	}
}

HoldEm::~HoldEm()
{
}

void HoldEm::Start()
{
	log("Starting game\n");
	assert(this->isPlaying == false);
	this->isPlaying = true;
	this->session.NewSession(this->players);
}

void HoldEm::Stop()
{
	log("Game stopped\n");
	assert(this->isPlaying == true);
	this->isPlaying = false;
	this->session.complete = true;
}

bool HoldEm::Update(int& gamesPlayed)
{
	assert(gamesPlayed >= 0);
	assert(this->isPlaying == true);

	if (this->session.complete)
	{
		RemoveLosingPlayers();
		this->session.NewSession(this->players);
		gamesPlayed++;
	}
	else
	{
		this->session.NextTurn();

		if (!this->session.complete)
		{
			this->session.HandlePlayerChoices();
		}
	}

	if (this->numPlayers < 2)
	{
		Stop();
	}

	return this->isPlaying;
}

char HoldEm::AddPlayer()
{
	assert(this->numPlayers < MAX_PLAYERS);

	for (char i = 0; i < MAX_PLAYERS; ++i)
	{
		Player& p = players[i];

		if (p.isActive == false)
		{
			p.Activate(i, STARTING_CHIPS, new RandomChoiceMaker());
			p.Play();
			this->numPlayers++;
			return i;
		}
	}

	assert(false);
	return 0;
}

void HoldEm::RemovePlayer(char player)
{
	assert(this->numPlayers > 0);
	assert(player < MAX_PLAYERS);
	assert(player >= 0);

	this->players[player].Deactivate();
	this->numPlayers--;
}