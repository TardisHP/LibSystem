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
	LibrarySystem(std::string bookTreeFile = "Dataset/bookTree.dat", std::string bookTreeNodeFile = "Dataset/bookTreeNode.dat", 
		std::string borrowTreeFile = "Dataset/borrowTree.dat", std::string borrowTreeNodeFile = "Dataset/borrowTreeNode.dat",
		std::string cardTreeFile = "Dataset/cardTree.dat", std::string cardTreeNodeFile = "Dataset/cardTreeNode.dat");
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
	// ͼ���
	std::string bookDatasetFile;
	std::string bookTreeFile;
	std::string bookTreeNodeFile;
	BpTree bookBpTree;
	// �����¼
	std::string borrowDatasetFile;
	std::string borrowTreeFile;
	std::string borrowTreeNodeFile;
	BpTree borrowBpTree;
	// ����֤��
	std::string cardDatasetFile;
	std::string cardTreeFile;
	std::string cardTreeNodeFile;
	BpTree cardBpTree;

	int findSameBook(Book& _book, std::fstream& fin);
};

#endif // !_LIBRARY_SYSTEM_H
