#include <functional>
#include "Borrow.h"

Borrow::Borrow()
{
	borrow_id = -1;
	card_id = -1;
	book_id = -1;
	borrow_date = 0;
	return_date = 0;
}

Borrow::Borrow(int _cardid, int _bookid)
	:card_id(_cardid), book_id(_bookid)
{
	borrow_date = 0;
	return_date = 0;
	borrow_id = std::hash<int>()(card_id) ^ (std::hash<int>()(book_id) << 1);
}