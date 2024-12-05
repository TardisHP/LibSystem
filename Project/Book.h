#ifndef _BOOK_H
#define _BOOK_H

#include <string>

#define MAX_LEN 63

class Book
{
public:
	int book_id;
	char title[MAX_LEN];
	char category[MAX_LEN];
	char author[MAX_LEN];
	char publisher[MAX_LEN];
	int publish_year;
	float price;
	int stock;

	Book();
	Book(const char* _ti, const char* _ca, const char* _au, const char* _pub, int _year, float _pr);
};

#endif // !_BOOK_H