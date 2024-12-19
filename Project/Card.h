#ifndef _CARD_H
#define _CARD_H

#include "Book.h"

class Card
{
public:
	int card_id;				//卡号
	char user_name[MAX_LEN];	//持卡人姓名
	char department[MAX_LEN];	//持卡人单位
	char type;					//持卡人身份

	Card();
	Card(const char* _na, const char* _dep, char _t = 'S');
	Card(const Card& _card);
};

#endif // !_CARD_H