#ifndef _BORROW_H
#define _BORROW_H

class Borrow
{
public:
	int user_id;
	int book_id;
	int borrow_date;
	int return_date;
	Borrow(int _userid, int _bookid, int _bdate, int _rdate)
		:user_id(_userid), book_id(_bookid), borrow_date(_bdate), return_date(_rdate)
	{
	}
	//int returnTimeCount();
};

#endif // !_BORROW_H