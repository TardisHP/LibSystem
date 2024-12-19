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

//图书管理系统接口
class LibrarySystem
{
public:
	LibrarySystem(std::string bookTreeFile = "Dataset/bookTree.dat", std::string bookTreeNodeFile = "Dataset/bookTreeNode.dat", 
		std::string borrowTreeFile = "Dataset/borrowTree.dat", std::string borrowTreeNodeFile = "Dataset/borrowTreeNode.dat",
		std::string cardTreeFile = "Dataset/cardTree.dat", std::string cardTreeNodeFile = "Dataset/cardTreeNode.dat",
		std::string hashTreeFile = "Dataset/hashTree.dat", std::string hashTreeNodeFile = "Dataset/hashTreeNode.dat");
	void run();

	// 图书入库模块。向图书库中注册(添加)一本新书，
	// 并返回新书的书号。如果该书已经存在于图书库中，那么入库操作将失败。
	// 当且仅当书的<类别, 书名, 出版社, 年份, 作者>均相同时，才认为两本书相同。
	// 请注意，book_id作为自增列，应该插入时由数据库生成。
	// 插入完成后，需要根据数据库生成的book_id值去更新book对象里的book_id。
	int storeBook(Book& book);

	// 图书批量入库模块。读取文件批量入库。
	int storeBooks(std::string path);

	// 图书查询模块
	int queryBook(std::vector<Book>& books, int k);
	int queryBook(std::vector<Book>& books, int year_l, int year_r);
	int queryBook(std::vector<Book>& books, float price_l, float price_r);
	int queryBook(std::vector<Book>& books, const char* str, QUERY_TYPE type);

	// 图书删除模块，从图书库中删除一本书。如果还有人尚未归还这本书，那么删除操作将失败。
	int removeBook(int k);

	// 图书增加库存模块。为图书库中的某一本书增加库存。其中库存增量deltaStock可正可负，若为负数，则需要保证最终库存是一个非负数。
	int incBookStock(int k, int deltaStock);

	// 图书修改模块。修改已入库图书的基本信息，该接口不能修改图书的书号和存量。
	int modifyBookInfo(Book _book);

	// 借书模块。根据给定的书号、卡号和借书时间添加一条借书记录，然后更新库存。若用户此前已经借过这本书但尚未归还，那么借书操作将失败。
	int borrowBook(Borrow& borrow);

	// 还书模块。根据给定的书号、卡号和还书时间，查询对应的借书记录，并补充归还时间，然后更新库存。
	int returnBook(Borrow& borrow);

	// 借书记录查询模块。查询某个用户的借书记录，按照借书时间递减、书号递增的方式排序。
	void showBorrowHistory(int cardId);

	// 借书证注册模块。注册一个借书证，若借书证已经存在，则该操作将失败。当且仅当<姓名, 单位, 身份>均相同时，才认为两张借书证相同。
	int registerCard(Card& card);

	// 注销借书证模块。如果该借书证还有未归还的图书，那么删除操作将失败。
	int removeCard(int cardId);

	// 借书证查询模块。列出所有的借书证。
	void showCards();
private:
	// 图书库
	std::string bookDatasetFile;	// 存储图书数据的文件路径。
	std::string bookTreeFile;		// 与图书数据相关的树结构文件的路径。用于树的头节点等信息。
	std::string bookTreeNodeFile;	// 图书树结构节点的文件路径。用于存储树结构的节点信息。
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
	int showInfo(std::vector<Book>& books);
};

#endif // !_LIBRARY_SYSTEM_H
