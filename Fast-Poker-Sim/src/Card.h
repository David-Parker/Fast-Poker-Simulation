#pragma once
#include <cassert>

/* Represents a card in a standard 52-card deck. */
struct Card
{
	static char* cards[52];

	char val;

	Card() : val(0) {}

	inline static void Swap(Card* a, Card* b)
	{
		char tempVal = a->val;
		a->val = b->val;
		b->val = tempVal;
	}

	inline void Assign(char value)
	{
		assert(value >= 0);
		assert(value <= 51);

		this->val = value;
	}
};