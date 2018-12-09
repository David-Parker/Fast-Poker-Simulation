#pragma once
#include <cassert>

/* Represents a card in a standard 52-card deck. */
struct Card
{
	const static char Jack = 11;
	const static char Queen = 12;
	const static char King = 13;
	const static char Ace = 14;
	const static char Spades = 1;
	const static char Hearts = 2;
	const static char Diamonds = 3;
	const static char Clubs = 4;

	static char* ranks[14];
	static char* suits[4];

	char rank; // J = 11, Q = 12, K = 13, A = 14
	char suit; // 1 = Spades, 2 = Hearts, 3 = Diamonds, 4 = Clubs

	Card() : rank(0), suit(0) {}

	inline static void Swap(Card* a, Card* b)
	{
		char tempRank = a->suit;
		char tempValue = a->rank;
		a->suit = b->suit;
		a->rank = b->rank;
		b->suit = tempRank;
		b->rank = tempValue;
	}

	inline void Assign(char value, char rank)
	{
		assert(value >= 2);
		assert(value <= 14);
		assert(rank >= 1);
		assert(rank <= 4);

		this->rank = value;
		this->suit = rank;
	}
};