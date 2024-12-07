#include <cstring>
#include "Card.h"

Card::Card()
{
	card_id = -1;
	user_name[0] = '\0';
	department[0] = '\0';
	type = 'S';
}

Card::Card(const char* _na, const char* _dep, char _t)
{
	card_id = -1;
	strcpy_s(user_name, _na);
	strcpy_s(department, _dep);
	if (_t != 'S' && _t != 'T')
		type = 'S';
	type = _t;
}
Card::Card(const Card& _card)
{
	card_id = _card.card_id;
	strcpy_s(user_name, _card.user_name);
	strcpy_s(department, _card.department);
	type = _card.type;
}