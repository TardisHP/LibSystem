#ifndef _LIBRARY_SYSTEM_H
#define _LIBRARY_SYSTEM_H

#include <string>
#include <vector>

#include "BpTree.h"
#include "Book.h"
#include "Card.h"
#include "Borrow.h"

#define MAX_BOOK_NUM 15000000

class LibrarySystem
{
public:
	LibrarySystem(std::string bookTreeFile = "Dataset/bookTree.dat", std::string bookTreeNodeFile = "Dataset/bookTreeNode.dat", 
		std::string borrowTreeFile = "Dataset/borrowTree.dat", std::string borrowTreeNodeFile = "Dataset/borrowTreeNode.dat",
		std::string cardTreeFile = "Dataset/cardTree.dat", std::string cardTreeNodeFile = "Dataset/cardTreeNode.dat",
		std::string hashTreeFile = "Dataset/hashTree.dat", std::string hashTreeNodeFile = "Dataset/hashTreeNode.dat");
	void run();
	void storeBook(Book& book);
	void storeBooks(std::string path);
	void queryBook(int k);
	void queryBook(int year_l, int year_r);
	void queryBook(float price_l, float price_r);
	void queryBook(const char* str, int type);
	void removeBook(int k);
	int incBookStock(int k, int deltaStock);
	void modifyBookInfo(Book _book);
	void borrowBook(Borrow& borrow);
	void returnBook(Borrow& borrow);
	void showBorrowHistory(int cardId);
	void registerCard(Card& card);
	void removeCard(int cardId);
	void showCards();
private:
	// 图书库
	std::string bookDatasetFile;
	std::string bookTreeFile;
	std::string bookTreeNodeFile;
	BpTree bookBpTree;
	// 借书记录
	std::string borrowDatasetFile;
	std::string borrowTreeFile;
	std::string borrowTreeNodeFile;
	BpTree borrowBpTree;
	// 借书证库
	std::string cardDatasetFile;
	std::string cardTreeFile;
	std::string cardTreeNodeFile;
	BpTree cardBpTree;
	// 字符串哈希，快速判断重复
	std::string hashTreeFile;
	std::string hashTreeNodeFile;
	BpTree hashBpTree;

	int findSameBook(Book& _book);
};

#endif // !_LIBRARY_SYSTEM_H
