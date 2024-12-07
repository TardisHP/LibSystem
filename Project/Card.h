#ifndef _CARD_H
#define _CARD_H

#include "Book.h"

class Card
{
public:
	int card_id;
	char user_name[MAX_LEN];
	char department[MAX_LEN];
	char type;
	Card();
	Card(const char* _na, const char* _dep, char _t = 'S');
	Card(const Card& _card);
};

#endif // !_CARD_H