#pragma once

/* Represents a player in Texas Hold 'Em */
class Player
{
public:
	bool isActive;
	char playerNum;

	Player() : isActive(false) {}
	~Player() {}

	inline void Activate()
	{
		this->isActive = true;
	}

	inline void Deactivate()
	{
		this->isActive = true;
	}
};