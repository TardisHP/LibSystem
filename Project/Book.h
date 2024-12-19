#ifndef _BOOK_H
#define _BOOK_H

#define MAX_LEN 63

class Book
{
public:
	int book_id;				//书号
	char title[MAX_LEN];		//书名
	char category[MAX_LEN];		//类别
	char author[MAX_LEN];		//作者
	char publisher[MAX_LEN];	//出版社
	int publish_year;			//年份
	float price;				//价格
	int stock;					//剩余库存

	Book();
	Book(const char* _ti, const char* _ca, const char* _au, const char* _pub, int _year, float _pr);
	Book(const Book& b);
};

#endif // !_BOOK_H