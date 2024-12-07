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
	LibrarySystem(std::string bookTreeFile = "Dataset/bookTree.dat", std::string bookTreeNodeFile = "Dataset/bookTreeNode.dat");
	void run();
	void storeBook(Book& book);
	void storeBooks(std::vector<Book>& books);
	void queryBook(int k);
	void queryBook(int k, int y);
	void removeBook(int k);
	void incBookStock(int k, int deltaStock);
	void modifyBookInfo(Book book);
private:
	std::string bookDatasetFile;
	std::string bookTreeFile;
	std::string bookTreeNodeFile;
	BpTree bookBpTree;
	int keyMax;
};

#endif // !_LIBRARY_SYSTEM_H
