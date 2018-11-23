#include <cassert>
#include "HoldEm.h"

HoldEm::HoldEm(char numPlayers) : numPlayers(numPlayers), isPlaying(false), players(), session()
{
	assert(numPlayers >= 2);
	assert(numPlayers <= maxPlayers);

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
	assert(this->isPlaying == false);
	this->isPlaying = true;
}

void HoldEm::Stop()
{
	assert(this->isPlaying == true);
	this->isPlaying = false;
	this->session.complete = true;
}

bool HoldEm::Update(int& gamesPlayed)
{
	assert(gamesPlayed >= 0);

	if (this->session.complete)
	{
		this->session.NewSession();
		gamesPlayed++;
	}
	else
	{
		this->session.NextTurn();
	}

	return this->isPlaying;
}

char HoldEm::AddPlayer()
{
	assert(numPlayers < maxPlayers);

	for (char i = 0; i < numPlayers; ++i)
	{
		Player& p = players[i];

		if (p.isActive == false)
		{
			p.Activate();
			numPlayers++;
			return i;
		}
	}

	assert(false);
	return 0;
}

void HoldEm::RemovePlayer(char player)
{
	assert(numPlayers > 0);
	assert(player < maxPlayers);
	assert(player >= 0);

	players[player].Deactivate();
	numPlayers--;
}