#ifndef _CARD_H
#define _CARD_H

#include <string>

class Card
{
public:
	int card_id;
	std::string user_name;
	std::string department;
	char type;
	Card(int _id, std::string _na, std::string _dep, char _ty) :
		card_id(_id), user_name(_na), department(_dep), type(_ty)
	{
	}
};

#endif // !_CARD_H