#ifndef _BORROW_H
#define _BORROW_H

#include <chrono>

class Borrow
{
public:
	int borrow_id;				//借书记录
	int card_id;				//卡号
	int book_id;				//书号
	std::time_t borrow_date;	//借书日期
	std::time_t return_date;	//还书日期
	
	Borrow();
	Borrow(int _cardid, int _bookid);
};

#endif // !_BORROW_H