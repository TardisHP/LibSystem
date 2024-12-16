#ifndef _LIBRARY_SYSTEM_H
#define _LIBRARY_SYSTEM_H

#include <string>
#include <vector>

#include "BpTree.h"
#include "Book.h"
#include "Card.h"
#include "Borrow.h"

#define MAX_BOOK_NUM 15000000

enum QUERY_TYPE { BY_CATEGORY, BY_TITLE, BY_PUBLISHER, BY_AUTHOR };

class LibrarySystem
{
public:
	LibrarySystem(std::string bookTreeFile = "Dataset/bookTree.dat", std::string bookTreeNodeFile = "Dataset/bookTreeNode.dat", 
		std::string borrowTreeFile = "Dataset/borrowTree.dat", std::string borrowTreeNodeFile = "Dataset/borrowTreeNode.dat",
		std::string cardTreeFile = "Dataset/cardTree.dat", std::string cardTreeNodeFile = "Dataset/cardTreeNode.dat",
		std::string hashTreeFile = "Dataset/hashTree.dat", std::string hashTreeNodeFile = "Dataset/hashTreeNode.dat");
	void run();
	int storeBook(Book& book);
	int storeBooks(std::string path);
	int queryBook(std::vector<Book>& books, int k);
	int queryBook(std::vector<Book>& books, int year_l, int year_r);
	int queryBook(std::vector<Book>& books, float price_l, float price_r);
	int queryBook(std::vector<Book>& books, const char* str, QUERY_TYPE type);
	int removeBook(int k);
	int incBookStock(int k, int deltaStock);
	int modifyBookInfo(Book _book);
	int borrowBook(Borrow& borrow);
	int returnBook(Borrow& borrow);
	void showBorrowHistory(int cardId);
	int registerCard(Card& card);
	int removeCard(int cardId);
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
	void showInfo(std::vector<Book>& books);
};

#endif // !_LIBRARY_SYSTEM_H
