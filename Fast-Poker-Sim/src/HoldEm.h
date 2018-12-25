#pragma once
#include <vector>
#include "Player.h"
#include "GameConfiguration.h"
#include "GameSession.h"

/* Simulation of Texas Hold 'Em no limits. */
class HoldEm
{
private:
	bool isPlaying;
	char totalPlayers;
	char numPlayingPlayers;
	Player players[MAX_PLAYERS];
	GameSession session;

	void RemoveLosingPlayers();

public:
	HoldEm(char numPlayers);
	~HoldEm();

	void NewGame();
	void Start();
	void Stop();

	bool Update(uint64_t& gamesPlayed);
	char AddPlayer();
	void RemovePlayer(char player);
};

