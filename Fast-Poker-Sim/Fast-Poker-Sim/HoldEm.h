#pragma once
#include <vector>
#include "Player.h"
#include "GameSession.h"

/* Simulation of Texas Hold 'Em no limits. */
class HoldEm
{
private:
	const int maxPlayers = 8;
	bool isPlaying;
	char numPlayers;
	Player players[8];
	GameSession session;

public:
	HoldEm(char numPlayers);
	~HoldEm();

	void Start();
	void Stop();

	bool Update(int& gamesPlayed);
	char AddPlayer();
	void RemovePlayer(char player);
};

