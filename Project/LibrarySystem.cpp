#define _CRT_SECURE_NO_WARNINGS

#include <string>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <chrono>
#include <random>
#include <sstream>

#include "LibrarySystem.h"
#include "Book.h"
#include "Card.h"
#include "Borrow.h"

inline void eatline() { while (std::cin.get() != '\n') continue; }

static Book queryIterBooks[MAX_LEAF];

LibrarySystem::LibrarySystem(std::string bookTreeFile, std::string bookTreeNodeFile, 
	std::string borrowTreeFile, std::string borrowTreeNodeFile,
	std::string cardTreeFile, std::string cardTreeNodeFile,
	std::string hashTreeFile, std::string hashTreeNodeFile) 
	: bookBpTree(bookTreeFile, bookTreeNodeFile), borrowBpTree(borrowTreeFile, borrowTreeNodeFile), 
	  cardBpTree(cardTreeFile, cardTreeNodeFile), hashBpTree(hashTreeFile, hashTreeNodeFile)
{
	bookDatasetFile = "Dataset/bookDataset.dat";
	borrowDatasetFile = "Dataset/borrowDataset.dat";
	cardDatasetFile = "Dataset/cardDataset.dat";
}

void message()
{
	std::cout << "-------------------------------------------------------------" << std::endl
		<< "s: store a book\t\ti: change inc\t\tr: remove a book" << std::endl
		<< "q: query books\t\tm: modify a book info" << std::endl
		<< "b: borrow a book\tt: return a book\th: show borrow history" << std::endl
		<< "c: regester a card\to: remove a card\tp: show all cards" << std::endl
		<< "e: exit" << std::endl
		<< "-------------------------------------------------------------" << std::endl;
}

template <typename T>
int getValidatedInput(const std::string& prompt, T& val)
{
	while (true) {
		std::cout << prompt;
		std::cin >> val;
		// 检查输入是否成功
		if (std::cin.fail()) 
		{
			std::cin.clear();
			// 忽略剩余的输入直到换行符
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << "wrong param" << std::endl;
		}
		else 
		{
			// 忽略剩余的输入直到换行符，防止多余输入影响下次读取
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			return 1;
		}
	}
}

void LibrarySystem::run()
{
	int status;	// 状态码
	char input;	// 输入
	std::vector<Book> books;

	std::cout << "###        Welcome To XZL Library Management System       ###" << std::endl;
	message();
	while (getValidatedInput<char>(">> ", input))
	{
		status = -1;
		switch (input)
		{
		case 's':
		{
			Book book;
			std::cout << "Enter the book title:" << std::endl;
			std::cout << ">> ";
			std::cin.getline(book.title, MAX_LEN);
			std::cout << "Enter the book category:" << std::endl;
			std::cout << ">> ";
			std::cin.getline(book.category, MAX_LEN);
			std::cout << "Enter the book author:" << std::endl;
			std::cout << ">> ";
			std::cin.getline(book.author, MAX_LEN);
			std::cout << "Enter the book publisher:" << std::endl;
			std::cout << ">> ";
			std::cin.getline(book.publisher, MAX_LEN);
			std::cout << "Enter the book year:" << std::endl;
			getValidatedInput<int>(">> ", book.publish_year);
			std::cout << "Enter the book price:" << std::endl;
			getValidatedInput<float>(">> ", book.price);
			std::cout << "Enter the book stock:" << std::endl;
			getValidatedInput<int>(">> ", book.stock);
			status = storeBook(book);
			break;
		}
		case 'i':
		{
			int id, stock;
			std::cout << "Enter the book id:" << std::endl;
			getValidatedInput<int>(">> ", id);
			std::cout << "Enter the delta stock:" << std::endl;
			getValidatedInput<int>(">> ", stock);
			status = incBookStock(id, stock);
			break;
		}
		case 'r':
		{
			int id;
			std::cout << "Enter the book id:" << std::endl;
			getValidatedInput<int>(">> ", id);
			status = removeBook(id);
			break;
		}
		case 'm':
		{
			Book book;
			std::cout << "Enter the book id:" << std::endl;
			getValidatedInput<int>(">> ", book.book_id);
			std::cout << "Enter the book title:" << std::endl;
			std::cout << ">> ";
			std::cin.getline(book.title, MAX_LEN);
			std::cout << "Enter the book category:" << std::endl;
			std::cout << ">> ";
			std::cin.getline(book.category, MAX_LEN);
			std::cout << "Enter the book author:" << std::endl;
			std::cout << ">> ";
			std::cin.getline(book.author, MAX_LEN);
			std::cout << "Enter the book publisher:" << std::endl;
			std::cout << ">> ";
			std::cin.getline(book.publisher, MAX_LEN);
			std::cout << "Enter the book year:" << std::endl;
			getValidatedInput<int>(">> ", book.publish_year);
			std::cout << "Enter the book price:" << std::endl;
			getValidatedInput<float>(">> ", book.price);
			status = modifyBookInfo(book);
			break;
		}
		case 'q':
		{
			int k, s;
			char str[MAX_LEN];
			std::cout << "-------------------------------------------------------------" << std::endl
				<< "1: by id\t\t2: by category\t\t3: by title" << std::endl
				<< "4: by publisher\t\t5: by year\t\t6: by author" << std::endl
				<< "7: by price" << std::endl
				<< "0: exit" << std::endl
				<< "-------------------------------------------------------------" << std::endl;
			getValidatedInput<int>(">> ", k);
			switch (k)
			{
			case 0:
				break;
			case 1:
				getValidatedInput<int>(">> id: ", k);
				status = queryBook(books, k);
				break;
			case 2:
				std::cout << ">> category: ";
				std::cin >> str;
				eatline();
				status = queryBook(books, str, BY_CATEGORY);
				break;
			case 3:
				std::cout << ">> title: ";
				std::cin.getline(str, MAX_LEN);
				status = queryBook(books, str, BY_TITLE);
				break;
			case 4:
				std::cout << ">> publisher: ";
				std::cin.getline(str, MAX_LEN);
				status = queryBook(books, str, BY_PUBLISHER);
				break;
			case 5:
				getValidatedInput<int>(">> year: ", k);
				getValidatedInput<int>(">> to year: ", s);
				status = queryBook(books, k, s);
				break;
			case 6:
				std::cout << ">> author: ";
				std::cin.getline(str, MAX_LEN);
				status = queryBook(books, str, BY_AUTHOR);
				break;
			case 7:
			{
				float k, s;
				getValidatedInput<float>(">> price: ", k);
				getValidatedInput<float>(">> to price: ", s);
				status = queryBook(books, k, s);
				break;
			}
			default:
				std::cout << "wrong param!" << std::endl;
				break;
			}
			break;
		}
		case 'b':
		{
			int cid, bid;
			std::cout << "Enter the card id:" << std::endl;
			getValidatedInput<int>(">> ", cid);
			std::cout << "Enter the book id:" << std::endl;
			getValidatedInput<int>(">> ", bid);
			Borrow b(cid, bid);
			status = borrowBook(b);
			break;
		}
		case 't':
		{
			int cid, bid;
			std::cout << "Enter the card id:" << std::endl;
			getValidatedInput<int>(">> ", cid);
			std::cout << "Enter the book id:" << std::endl;
			getValidatedInput<int>(">> ", bid);
			Borrow b(cid, bid);
			status = returnBook(b);
			break;
		}
		case 'h':
			int cid, bid;
			std::cout << "Enter the card id:" << std::endl;
			getValidatedInput<int>(">> ", cid);
			showBorrowHistory(cid);
			break;
		case 'c':
		{
			char na[MAX_LEN];
			char dp[MAX_LEN];
			char id;
			std::cout << "Enter your name:" << std::endl;
			std::cout << ">> ";
			std::cin.getline(na, MAX_LEN);
			std::cout << "Enter your departure:" << std::endl;
			std::cout << ">> ";
			std::cin.getline(dp, MAX_LEN);
			std::cout << "Enter your identity (S/T):" << std::endl;
			getValidatedInput<char>(">> ", id);
			Card card(na, dp, id);
			status = registerCard(card);
			break;
		}
		case 'o':
		{
			int cid;
			std::cout << "Enter the card id:" << std::endl;
			getValidatedInput<int>(">> ", cid);
			status = removeCard(cid);
			break;
		}
		case 'p':
			showCards();
			break;
		case 'e':
			return;
		default:
			std::cout << "wrong param!" << std::endl;
			break;
		}

		switch (status)
		{
		case -1:
			break;
		case 100:
			std::cout << "insert SUCCESS!" << std::endl;
			break;
		case 101:
			break;
		case 102:
			std::cout << "modify SUCCESS!" << std::endl;
			break;
		case 103:
			std::cout << "delete SUCCESS!" << std::endl;
			break;
		case 104:
			std::cout << "stock change SUCCESS!" << std::endl;
			break;
		case 105:
			std::cout << "borrow SUCCESS!" << std::endl;
			break;
		case 106:
			std::cout << "return SUCCESS!" << std::endl;
			break;
		case 107:
			std::cout << "register SUCCESS!" << std::endl;
			break;
		case 200:
			std::cout << "query QUIT!" << std::endl;
			break;
		case 400:
			std::cout << "REPEATED!" << std::endl;
			break;
		case 401:
			std::cout << "query FAIL!" << std::endl;
			break;
		case 402:
			std::cout << "delete FAIL!" << std::endl;
			break;
		case 403:
			std::cout << "stock is ZERO!" << std::endl;
			break;
		case 404:
			std::cout << "can NOT find the card!" << std::endl;
			break;
		case 405:
			std::cout << "has been borrowed!" << std::endl;
			break;
		case 406:
			std::cout << "has been returned!" << std::endl;
			break;
		case 407:
			std::cout << "register FAIL!" << std::endl;
			break;
		case 408:
			std::cout << "delete FAIL! There is a record of borrowed books that have not been returned!" << std::endl;
			break;
		default:
			std::cout << "book [ " << status << " ] has been stored!"<< std::endl;
			break;
		}
		message();
	}
}

int LibrarySystem::findSameBook(Book& _book)
{
	std::stringstream ss;
	ss << _book.title << _book.category << _book.author << _book.publisher << _book.publish_year;
	std::hash<std::string> hash_fn;
	size_t hash_value = hash_fn(ss.str());
	if (hashBpTree.findPos(hash_value) != -1)
		return 1;
	return 0;
}

int LibrarySystem::storeBook(Book& book)
{
	std::fstream finout;
	finout.open(bookDatasetFile, std::ios::in | std::ios::out | std::ios::binary | std::ios::ate);
	unsigned long v;
	
	if (findSameBook(book))
		return 400;

	if (finout.is_open())
	{
		// 生成id
		std::random_device rd;
		std::mt19937 engine(rd());
		std::uniform_int_distribution<int> dist(1000, MAX_BOOK_NUM);
		do
		{
			v = finout.tellp();
			book.book_id = dist(engine);
		} while (bookBpTree.insertToLeaf(book.book_id, v) == 0);
		// 哈希
		std::stringstream ss;
		ss << book.title << book.category << book.author << book.publisher << book.publish_year;
		std::hash<std::string> hash_fn;
		size_t hash_value = hash_fn(ss.str());
		hashBpTree.insertToLeaf(hash_value, 1);

		finout.seekp(0, std::ios::end);
		finout.write((char*)&book, sizeof book);
		finout.close();
	}
	return book.book_id;
}

void split(const std::string& s, std::vector<std::string>& tokens, const std::string& delimiters = " ")
{
	std::string::size_type lastPos = s.find_first_not_of(delimiters, 0);
	std::string::size_type pos = s.find_first_of(delimiters, lastPos);
	while (std::string::npos != pos || std::string::npos != lastPos) {
		tokens.push_back(s.substr(lastPos, pos - lastPos));//use emplace_back after C++11
		lastPos = s.find_first_not_of(delimiters, pos);
		pos = s.find_first_of(delimiters, lastPos);
	}
}

int LibrarySystem::storeBooks(std::string path)
{
	std::fstream origin, finout;
	origin.open(path, std::ios::in);
	if (origin.is_open())
	{
		std::fstream finout;
		finout.open(bookDatasetFile, std::ios::in | std::ios::out | std::ios::binary);
		if (finout.is_open())
		{
			Book book;
			std::string bookInfo;
			finout.seekp(0, std::ios::end);
			unsigned long pos = finout.tellp();
			unsigned long offset = sizeof book;
			int num = pos;
			while (getline(origin, bookInfo))
			{
				std::vector<std::string> tokens;
				split(bookInfo, tokens);

				book.book_id = num;
				strcpy_s(book.title, tokens[0].c_str());
				strcpy_s(book.category, tokens[1].c_str());
				strcpy_s(book.author, tokens[2].c_str());
				strcpy_s(book.publisher, tokens[3].c_str());
				book.publish_year = std::stoi(tokens[4]);
				book.price = std::stof(tokens[5]);
				book.stock = 5;
				
				finout.seekp(pos);
				finout.write((char*)&book, sizeof book);
				bookBpTree.insertToLeaf(num, pos);
				// 哈希
				std::stringstream ss;
				ss << book.title << book.category << book.author << book.publisher << book.publish_year;
				std::hash<std::string> hash_fn;
				size_t hash_value = hash_fn(ss.str());
				hashBpTree.insertToLeaf(hash_value, 1);

				num++;
				pos += offset;
				if (num % 10000 == 0)
					std::cout << "# " << num << " has benn finished..." << std::endl;
			}
			finout.close();
		}
		origin.close();
	}
	std::cout << "Store finished!" << std::endl;
	return 100;
}

int LibrarySystem::showInfo(std::vector<Book>& books)
{
	if (books.empty())
		return 0;
	else
	{
		std::cout << std::left << std::setw(15) << "ID" << std::setw(15) << "category" << std::setw(20) << "title" << std::setw(15) << "author" << std::setw(15)
			<< "publisher" << std::setw(10) << "year" << std::setw(10) << "price" << std::setw(10) << "stock" << std::endl;
		for (auto& book : books)
		{
			std::cout << std::left << std::setw(15) << book.book_id << std::setw(15) << book.category << std::setw(20) << book.title << std::setw(15) << book.author << std::setw(15)
				<< book.publisher << std::setw(10) << book.publish_year << std::setw(10) << book.price << std::setw(10) << book.stock << std::endl;
		}
		std::cout << std::endl;
		// 判断是否继续显示
		if (books.size() < 20)
		{
			books.clear();
			return 1;
		}
		books.clear();
		std::cout << "input 'q' to quit, else to continue" << std::endl;
		std::cout << ">> ";
		char c;
		std::cin >> c;
		eatline();
		if (c == 'q')
		{
			return 0;
		}
	}
	return 1;
}

int LibrarySystem::queryBook(std::vector<Book>& books, int k)
{
	Book book;
	unsigned long v = bookBpTree.findPos(k);
	if (v == -1)
		return 401;
	std::fstream fin;
	fin.open(bookDatasetFile, std::ios::in | std::ios::binary);
	if (fin.is_open())
	{
		fin.seekg(v);
		fin.read((char*)&book, sizeof book);
		books.push_back(book);
		fin.close();
	}
	if (!showInfo(books))
		return 401;
	return 101;
}

int LibrarySystem::queryBook(std::vector<Book>& books, int year_l, int year_r)
{
	Book book;
	std::fstream fin;
	std::vector<unsigned long> poses;
	unsigned long p = bookBpTree.head;
	poses = bookBpTree.iter(p);
	fin.open(bookDatasetFile, std::ios::in | std::ios::binary);
	if (fin.is_open())
	{
		int num = 0;
		while (!poses.empty())
		{
			fin.seekg(poses[0]);
			fin.read((char*)&queryIterBooks, (sizeof book) * poses.size());
			for (int i = 0; i < poses.size(); i++)
			{
				if (queryIterBooks[i].publish_year >= year_l && queryIterBooks[i].publish_year <= year_r)
				{
					books.push_back(queryIterBooks[i]);
					num++;
					if (num % 20 == 0 && num > 0)
					{
						if (!showInfo(books))
						{
							fin.close();
							return 200;
						}
					}
				}
			}
			poses = bookBpTree.iter(p);
		}
		fin.close();
	}
	if (!showInfo(books))
		return 401;
	return 101;
}

int LibrarySystem::queryBook(std::vector<Book>& books, float price_l, float price_r)
{
	Book book;
	std::fstream fin;
	std::vector<unsigned long> poses;
	unsigned long p = bookBpTree.head;
	poses = bookBpTree.iter(p);
	fin.open(bookDatasetFile, std::ios::in | std::ios::binary);
	if (fin.is_open())
	{
		int num = 0;
		while (!poses.empty())
		{
			fin.seekg(poses[0]);
			fin.read((char*)&queryIterBooks, (sizeof book) * poses.size());
			for (int i = 0; i < poses.size(); i++)
			{
				if (queryIterBooks[i].price >= price_l && queryIterBooks[i].price <= price_r)
				{
					books.push_back(queryIterBooks[i]);
					num++;
					if (num % 20 == 0 && num > 0)
					{
						if (!showInfo(books))
						{
							fin.close();
							return 200;
						}
					}
				}
			}
			poses = bookBpTree.iter(p);
		}
		fin.close();
	}
	if (!showInfo(books))
		return 401;
	return 101;
}

int LibrarySystem::queryBook(std::vector<Book>& books, const char* str, QUERY_TYPE type)
{
	Book book;
	std::fstream fin;
	std::vector<unsigned long> poses;
	unsigned long p = bookBpTree.head;
	poses = bookBpTree.iter(p);
	
	fin.open(bookDatasetFile, std::ios::in | std::ios::binary);
	if (fin.is_open())
	{
		int num = 0;
		while (!poses.empty())
		{
			fin.seekg(poses[0]);
			fin.read((char*)&queryIterBooks, (sizeof book)*poses.size());
			for (int i = 0; i < poses.size(); i++)
			{
				switch (type)
				{
				case BY_CATEGORY:
					if (std::strcmp(queryIterBooks[i].category, str) != 0)
						continue;
					break;
				case BY_TITLE:
					if (std::strstr(queryIterBooks[i].title, str) == nullptr)
						continue;
					break;
				case BY_PUBLISHER:
					if (std::strstr(queryIterBooks[i].publisher, str) == nullptr)
						continue;
					break;
				case BY_AUTHOR:
					if (std::strstr(queryIterBooks[i].author, str) == nullptr)
						continue;
					break;
				default:
					break;
				}
				books.push_back(queryIterBooks[i]);
				num++;
				if (num % 20 == 0 && num > 0)
				{
					if (!showInfo(books))
					{
						fin.close();
						return 200;
					}
				}
			}
			poses = bookBpTree.iter(p);
		}
		fin.close();
	}
	if (!showInfo(books))
		return 401;
	return 101;
}

int LibrarySystem::modifyBookInfo(Book _book)
{
	unsigned long pos = bookBpTree.findPos(_book.book_id);
	if (pos == -1)
		return 401;
	if (findSameBook(_book))
		return 400;

	Book book;
	std::fstream finout;
	finout.open(bookDatasetFile, std::ios::in | std::ios::out | std::ios::binary);
	if (finout.is_open())
	{
		finout.seekg(pos);
		finout.read((char*)&book, sizeof book);
		strcpy_s(book.title, _book.title);
		strcpy_s(book.category, _book.category);
		strcpy_s(book.author, _book.author);
		strcpy_s(book.publisher, _book.publisher);
		book.publish_year = _book.publish_year;
		book.price = _book.price;
		finout.seekp(pos);
		finout.write((char*)&book, sizeof book);
		finout.close();
	}
	return 102;
}

int LibrarySystem::removeBook(int k)
{
	if (bookBpTree.deleteLeaf(k))
		return 103;
	else
		return 402;
}

int LibrarySystem::incBookStock(int k, int deltaStock)
{
	Book book;
	unsigned long pos = bookBpTree.findPos(k);
	if (pos == -1)
		return 401;

	std::fstream finout;
	finout.open(bookDatasetFile, std::ios::in | std::ios::out | std::ios::binary);
	if (finout.is_open())
	{
		finout.seekg(pos);
		finout.read((char*)&book, sizeof book);
		book.stock += deltaStock;
		if (book.stock < 0)
		{
			finout.close();
			return 403;
		}
		finout.seekp(pos);
		finout.write((char*)&book, sizeof book);
		finout.close();
	}
	return 104;
}

int LibrarySystem::borrowBook(Borrow& borrow)
{
	if (cardBpTree.findPos(borrow.card_id) == -1)
		return 404;

	unsigned long pos;
	std::fstream finout;
	finout.open(borrowDatasetFile, std::ios::in | std::ios::out | std::ios::binary);
	if (finout.is_open())
	{
		if (borrowBpTree.findPos(borrow.borrow_id) != -1)
		{
			finout.close();
			return 405;
		}
		// 检查库存
		if (incBookStock(borrow.book_id, -1) == 0)
		{
			finout.close();
			return 403;
		}
		finout.seekp(0, std::ios::end);
		pos = finout.tellp();
		// 获取时间
		auto now = std::chrono::system_clock::now();
		std::time_t now_c = std::chrono::system_clock::to_time_t(now);
		borrow.borrow_date = now_c;
		finout.write((char*)&borrow, sizeof borrow);
		borrowBpTree.insertToLeaf(borrow.borrow_id, pos);
		finout.close();
	}
	return 105;
}

int LibrarySystem::returnBook(Borrow& borrow)
{
	if (cardBpTree.findPos(borrow.card_id) == -1)
		return 404;

	unsigned long pos = borrowBpTree.findPos(borrow.borrow_id);
	std::fstream finout;
	finout.open(borrowDatasetFile, std::ios::in | std::ios::out | std::ios::binary);
	if (finout.is_open())
	{
		if (pos == -1)
		{
			finout.close();
			return 401;
		}
		Borrow btmp;
		finout.seekg(pos);
		finout.read((char*)&btmp, sizeof btmp);
		// std::cout << "Borrowing Book..." << std::endl;
		if (btmp.return_date != 0)
		{
			finout.close();
			return 406;
		}
		// 获取时间
		auto now = std::chrono::system_clock::now();
		std::time_t now_c = std::chrono::system_clock::to_time_t(now);
		btmp.return_date = now_c;
		finout.seekp(pos);
		finout.write((char*)&btmp, sizeof btmp);
		incBookStock(borrow.book_id, 1);
		finout.close();
	}
	return 106;
}

void LibrarySystem::showBorrowHistory(int cardId)
{
	std::fstream fin;
	Borrow borrow;
	Pair p;
	p.k = borrowBpTree.head;
	p.v = 0;
	unsigned long pos = borrowBpTree.iter(p);
	fin.open(borrowDatasetFile, std::ios::in | std::ios::binary);
	std::cout << std::left << std::setw(15) << "cardID" << std::setw(15) << "bookID" << std::setw(30) << "borrow" << std::setw(30) << "return" <<  std::endl;
	if (fin.is_open())
	{
		while (pos != -1)
		{
			fin.seekg(pos);
			fin.read((char*)&borrow, sizeof borrow);
			if (borrow.card_id == cardId)
			{
				// 删去换行
				std::string time_s(std::ctime(&borrow.borrow_date));
				time_s.pop_back();

				std::cout << std::left << std::setw(15) << borrow.card_id << std::setw(15) << borrow.book_id << std::setw(30) << time_s << std::setw(30);
				if (borrow.return_date == 0)
					std::cout << "None" << std::endl;
				else
				{
					time_s = std::ctime(&borrow.return_date);
					time_s.pop_back();
					std::cout << time_s << std::endl;
				}
			}
			pos = borrowBpTree.iter(p);
		}
		fin.close();
	}
	std::cout << std::endl;
}

int LibrarySystem::registerCard(Card& card)
{
	if (card.card_id != -1)
		return 407;

	if (card.user_name[0] == '\0')
		return 407;

	std::fstream finout;
	Card ctmp;
	Pair p;
	p.k = cardBpTree.head;
	p.v = 0;
	unsigned long pos = cardBpTree.iter(p);
	finout.open(cardDatasetFile, std::ios::in | std::ios::out | std::ios::binary);
	if (finout.is_open())
	{
		while (pos != -1)
		{
			finout.seekg(pos);
			finout.read((char*)&ctmp, sizeof ctmp);
			if (strcmp(ctmp.user_name, card.user_name) == 0 && strcmp(ctmp.department, card.department) == 0 && ctmp.type == card.type)
			{
				finout.close();
				return 400;
			}
			pos = cardBpTree.iter(p);
		}
		finout.seekp(0, std::ios::end);
		pos = finout.tellp();
		// 生成id
		std::random_device rd;
		std::mt19937 engine(rd());
		std::uniform_int_distribution<int> dist(1, std::numeric_limits<int>::max());
		do
		{
			card.card_id = dist(engine);
		} while (cardBpTree.insertToLeaf(card.card_id, pos) == 0);
		finout.write((char*)&card, sizeof card);
		finout.close();
	}
	return 107;
}

int LibrarySystem::removeCard(int cardId)
{
	Borrow borrow;
	std::fstream fin;
	std::vector<unsigned long> poses;
	unsigned long p = bookBpTree.head;
	poses = borrowBpTree.iter(p);
	fin.open(borrowDatasetFile, std::ios::in | std::ios::binary);
	if (fin.is_open())
	{
		int num = 0;
		while (!poses.empty())
		{
			for (unsigned long pos : poses)
			{
				fin.seekg(pos);
				fin.read((char*)&borrow, sizeof borrow);
				if (borrow.card_id == cardId && borrow.return_date == 0)
				{
					fin.close();
					return 408;
				}
			}
			poses = borrowBpTree.iter(p);
		}
		fin.close();
	}
	if (cardBpTree.deleteLeaf(cardId))
		return 103;
	else
		return 402;
}

void LibrarySystem::showCards()
{
	std::fstream fin;
	Card card;
	Pair p;
	p.k = cardBpTree.head;
	p.v = 0;
	unsigned long pos = cardBpTree.iter(p);
	fin.open(cardDatasetFile, std::ios::in | std::ios::binary);
	std::cout << std::left << std::setw(15) << "cardID" << std::setw(15) << "name" << std::setw(15) << "department" << std::setw(15) << "type" << std::endl;
	if (fin.is_open())
	{
		while (pos != -1)
		{
			fin.seekg(pos);
			fin.read((char*)&card, sizeof card);
			std::cout << std::left << std::setw(15) << card.card_id << std::setw(15) << card.user_name << std::setw(15) << card.department << std::setw(15) << card.type << std::endl;
			pos = cardBpTree.iter(p);
		}
		fin.close();
	}
	std::cout << std::endl;
}