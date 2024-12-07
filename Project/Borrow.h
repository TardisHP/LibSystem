#ifndef _BORROW_H
#define _BORROW_H

#include <chrono>

class Borrow
{
public:
	int borrow_id;
	int card_id;
	int book_id;
	std::time_t borrow_date;
	std::time_t return_date;
	
	Borrow();
	Borrow(int _cardid, int _bookid);
};

#endif // !_BORROW_H