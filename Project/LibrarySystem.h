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

//ͼ�����ϵͳ�ӿ�
class LibrarySystem
{
public:
	LibrarySystem(std::string bookTreeFile = "Dataset/bookTree.dat", std::string bookTreeNodeFile = "Dataset/bookTreeNode.dat", 
		std::string borrowTreeFile = "Dataset/borrowTree.dat", std::string borrowTreeNodeFile = "Dataset/borrowTreeNode.dat",
		std::string cardTreeFile = "Dataset/cardTree.dat", std::string cardTreeNodeFile = "Dataset/cardTreeNode.dat",
		std::string hashTreeFile = "Dataset/hashTree.dat", std::string hashTreeNodeFile = "Dataset/hashTreeNode.dat");
	void run();

	// ͼ�����ģ�顣��ͼ�����ע��(���)һ�����飬
	// �������������š���������Ѿ�������ͼ����У���ô��������ʧ�ܡ�
	// ���ҽ������<���, ����, ������, ���, ����>����ͬʱ������Ϊ��������ͬ��
	// ��ע�⣬book_id��Ϊ�����У�Ӧ�ò���ʱ�����ݿ����ɡ�
	// ������ɺ���Ҫ�������ݿ����ɵ�book_idֵȥ����book�������book_id��
	int storeBook(Book& book);

	// ͼ���������ģ�顣��ȡ�ļ�������⡣
	int storeBooks(std::string path);

	// ͼ���ѯģ��
	int queryBook(std::vector<Book>& books, int k);
	int queryBook(std::vector<Book>& books, int year_l, int year_r);
	int queryBook(std::vector<Book>& books, float price_l, float price_r);
	int queryBook(std::vector<Book>& books, const char* str, QUERY_TYPE type);

	// ͼ��ɾ��ģ�飬��ͼ�����ɾ��һ���顣�����������δ�黹�Ȿ�飬��ôɾ��������ʧ�ܡ�
	int removeBook(int k);

	// ͼ�����ӿ��ģ�顣Ϊͼ����е�ĳһ�������ӿ�档���п������deltaStock�����ɸ�����Ϊ����������Ҫ��֤���տ����һ���Ǹ�����
	int incBookStock(int k, int deltaStock);

	// ͼ���޸�ģ�顣�޸������ͼ��Ļ�����Ϣ���ýӿڲ����޸�ͼ�����źʹ�����
	int modifyBookInfo(Book _book);

	// ����ģ�顣���ݸ�������š����źͽ���ʱ�����һ�������¼��Ȼ����¿�档���û���ǰ�Ѿ�����Ȿ�鵫��δ�黹����ô���������ʧ�ܡ�
	int borrowBook(Borrow& borrow);

	// ����ģ�顣���ݸ�������š����źͻ���ʱ�䣬��ѯ��Ӧ�Ľ����¼��������黹ʱ�䣬Ȼ����¿�档
	int returnBook(Borrow& borrow);

	// �����¼��ѯģ�顣��ѯĳ���û��Ľ����¼�����ս���ʱ��ݼ�����ŵ����ķ�ʽ����
	void showBorrowHistory(int cardId);

	// ����֤ע��ģ�顣ע��һ������֤��������֤�Ѿ����ڣ���ò�����ʧ�ܡ����ҽ���<����, ��λ, ���>����ͬʱ������Ϊ���Ž���֤��ͬ��
	int registerCard(Card& card);

	// ע������֤ģ�顣����ý���֤����δ�黹��ͼ�飬��ôɾ��������ʧ�ܡ�
	int removeCard(int cardId);

	// ����֤��ѯģ�顣�г����еĽ���֤��
	void showCards();
private:
	// ͼ���
	std::string bookDatasetFile;	// �洢ͼ�����ݵ��ļ�·����
	std::string bookTreeFile;		// ��ͼ��������ص����ṹ�ļ���·������������ͷ�ڵ����Ϣ��
	std::string bookTreeNodeFile;	// ͼ�����ṹ�ڵ���ļ�·�������ڴ洢���ṹ�Ľڵ���Ϣ��
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
	// �ַ�����ϣ�������ж��ظ�
	std::string hashTreeFile;
	std::string hashTreeNodeFile;
	BpTree hashBpTree;

	int findSameBook(Book& _book);
	int showInfo(std::vector<Book>& books);
};

#endif // !_LIBRARY_SYSTEM_H
