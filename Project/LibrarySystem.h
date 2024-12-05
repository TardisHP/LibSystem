#ifndef _LIBRARY_SYSTEM_H
#define _LIBRARY_SYSTEM_H

#include <string>
#include <vector>

#include "BpTree.h"
#include "Book.h"
#include "Card.h"
#include "Borrow.h"

class LibrarySystem
{
public:
	LibrarySystem();
	void storeBook(Book& book);
	void storeBooks(std::vector<Book>& books);
	void clearKV();
	void show();
	void queryBook(int k);
private:
	std::string kvFile;
	std::string datasetFile;
	BpTree bpTree;
	int keyMax;

	void createBpTreeFromFile();
	void saveBpTreeToFile();
};

#endif // !_LIBRARY_SYSTEM_H
