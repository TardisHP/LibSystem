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

void LibrarySystem::run()
{
	int status;	// 状态码
	char input;	// 输入
	std::vector<Book> books;

	std::cout << "###        Welcome To XZL Library Management System       ###" << std::endl;
	message();
	while (std::cin.get(input))
	{
		status = -1;
		eatline();
		switch (input)
		{
		case 's':
		{
			Book book;
			std::cout << "Enter the book title:" << std::endl;
			std::cin.getline(book.title, MAX_LEN);
			std::cout << "Enter the book category:" << std::endl;
			std::cin.getline(book.category, MAX_LEN);
			std::cout << "Enter the book author:" << std::endl;
			std::cin.getline(book.author, MAX_LEN);
			std::cout << "Enter the book publisher:" << std::endl;
			std::cin.getline(book.publisher, MAX_LEN);
			std::cout << "Enter the book year:" << std::endl;
			std::cin >> book.publish_year;
			std::cout << "Enter the book price:" << std::endl;
			std::cin >> book.price;
			std::cout << "Enter the book stock:" << std::endl;
			std::cin >> book.stock;
			status = storeBook(book);
			break;
		}
		case 'i':
		{
			int id, stock;
			std::cout << "Enter the book id:" << std::endl;
			std::cin >> id;
			std::cout << "Enter the delta stock:" << std::endl;
			std::cin >> stock;
			status = incBookStock(id, stock);
			break;
		}
		case 'r':
		{
			int id;
			std::cout << "Enter the book id:" << std::endl;
			std::cin >> id;
			status = removeBook(id);
			break;
		}
		case 'm':
		{
			Book book;
			std::cout << "Enter the book id:" << std::endl;
			std::cin >> book.book_id;
			eatline();
			std::cout << "Enter the book title:" << std::endl;
			std::cin.getline(book.title, MAX_LEN);
			std::cout << "Enter the book category:" << std::endl;
			std::cin.getline(book.category, MAX_LEN);
			std::cout << "Enter the book author:" << std::endl;
			std::cin.getline(book.author, MAX_LEN);
			std::cout << "Enter the book publisher:" << std::endl;
			std::cin.getline(book.publisher, MAX_LEN);
			std::cout << "Enter the book year:" << std::endl;
			std::cin >> book.publish_year;
			std::cout << "Enter the book price:" << std::endl;
			std::cin >> book.price;
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
			std::cin >> k;
			eatline();
			switch (k)
			{
			case 0:
				break;
			case 1:
				std::cin >> k;
				eatline();
				status = queryBook(books, k);
				break;
			case 2:
				std::cin >> str;
				eatline();
				status = queryBook(books, str, BY_CATEGORY);
				break;
			case 3:
				std::cin >> str;
				eatline();
				status = queryBook(books, str, BY_TITLE);
				break;
			case 4:
				std::cin >> str;
				eatline();
				status = queryBook(books, str, BY_PUBLISHER);
				break;
			case 5:
				std::cin >> k >> s;
				eatline();
				status = queryBook(books, k, s);
				break;
			case 6:
				std::cin >> str;
				eatline();
				status = queryBook(books, str, BY_AUTHOR);
				break;
			case 7:
			{
				float k, s;
				std::cin >> k >> s;
				eatline();
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
			std::cin >> cid;
			std::cout << "Enter the book id:" << std::endl;
			std::cin >> bid;
			Borrow b(cid, bid);
			status = borrowBook(b);
			break;
		}
		case 't':
		{
			int cid, bid;
			std::cout << "Enter the card id:" << std::endl;
			std::cin >> cid;
			std::cout << "Enter the book id:" << std::endl;
			std::cin >> bid;
			Borrow b(cid, bid);
			status = returnBook(b);
			break;
		}
		case 'h':
			showBorrowHistory(123);
			break;
		case 'c':
		{
			char na[50];
			char dp[50];
			char id;
			std::cout << "Enter your name:" << std::endl;
			std::cin.get(na, 50);
			eatline();
			std::cout << "Enter your departure:" << std::endl;
			std::cin.get(dp, 50);
			eatline();
			std::cout << "Enter your identity:" << std::endl;
			std::cin >> id;
			eatline();
			Card card(na, dp, id);
			status = registerCard(card);
			break;
		}
		case 'o':
		{
			int cid;
			std::cout << "Enter the card id:" << std::endl;
			std::cin >> cid;
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

void LibrarySystem::showInfo(std::vector<Book>& books)
{
	if (books.empty())
		std::cout << "can NOT find!" << std::endl;
	else
	{
		std::cout << "ID" << std::setw(20) << "category" << std::setw(20) << "title" << std::setw(15) << "author" << std::setw(15)
			<< "publisher" << std::setw(15) << "year" << std::setw(10) << "price" << std::setw(10) << "stock" << std::endl;
		for (auto& book : books)
		{
			std::cout << book.book_id << std::setw(20) << book.category << std::setw(20) << book.title << std::setw(15) << book.author << std::setw(15)
				<< book.publisher << std::setw(15) << book.publish_year << std::setw(10) << book.price << std::setw(10) << book.stock << std::endl;
		}
		std::cout << std::endl;
		books.clear();
	}
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
		showInfo(books);
		fin.close();
	}
	return 101;
}

int LibrarySystem::queryBook(std::vector<Book>& books, int year_l, int year_r)
{
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
			Book book;
			for (unsigned long pos : poses)
			{
				fin.seekg(pos);
				fin.read((char*)&book, sizeof book);
				if (book.publish_year >= year_l && book.publish_year <= year_r)
				{
					books.push_back(book);
					num++;
					if (num % 20 == 0 && num > 0)
					{
						showInfo(books);
						char c;
						std::cin >> c;
						eatline();
						if (c == 'q')
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
	showInfo(books);
	return 101;
}

int LibrarySystem::queryBook(std::vector<Book>& books, float price_l, float price_r)
{
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
			Book book;
			for (unsigned long pos : poses)
			{
				fin.seekg(pos);
				fin.read((char*)&book, sizeof book);
				if (book.publish_year >= price_l && book.publish_year <= price_r)
				{
					books.push_back(book);
					num++;
					if (num % 20 == 0 && num > 0)
					{
						showInfo(books);
						char c;
						std::cin >> c;
						eatline();
						if (c == 'q')
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
	showInfo(books);
	return 101;
}

int LibrarySystem::queryBook(std::vector<Book>& books, const char* str, QUERY_TYPE type)
{
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
			Book book;
			for (unsigned long pos : poses)
			{
				fin.seekg(pos);
				fin.read((char*)&book, sizeof book);
				switch (type)
				{
				case 0:
					if (std::strcmp(book.category, str) != 0)
						continue;
					break;
				case 1:
					if (std::strstr(book.title, str) == nullptr)
						continue;
					break;
				case 2:
					if (std::strstr(book.publisher, str) == nullptr)
						continue;
					break;
				case 3:
					if (std::strstr(book.author, str) == nullptr)
						continue;
					break;
				default:
					break;
				}
				books.push_back(book);
				num++;
				if (num % 20 == 0 && num > 0)
				{
					showInfo(books);
					std::cout << "input 'q' to quit, else to continue" << std::endl;
					char c;
					std::cin >> c;
					eatline();
					if (c == 'q')
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
	showInfo(books);
	return 101;
}

int LibrarySystem::modifyBookInfo(Book _book)
{
	unsigned long pos = bookBpTree.findPos(_book.book_id);
	if (pos == -1)
		return 401;

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
	std::cout << "Return SUCCESSED!" << std::endl;
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
	std::cout << "cardID" << std::setw(15) << "bookID" << std::setw(30) << "borrow" << std::setw(30) << "return" <<  std::endl;
	if (fin.is_open())
	{
		while (pos != -1)
		{
			fin.seekg(pos);
			fin.read((char*)&borrow, sizeof borrow);
			// 删去换行
			std::string time_s(std::ctime(&borrow.borrow_date));
			time_s.pop_back();
			std::cout << borrow.card_id << std::setw(15) << borrow.book_id << std::setw(30) << time_s << std::setw(30);
			if (borrow.return_date == 0)
				std::cout << "None" << std::endl;
			else
			{
				time_s = std::ctime(&borrow.return_date);
				time_s.pop_back();
				std::cout << time_s << std::endl;
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
	std::cout << "cardID" << std::setw(15) << "name" << std::setw(15) << "department" << std::setw(15) << "type" << std::endl;
	if (fin.is_open())
	{
		while (pos != -1)
		{
			fin.seekg(pos);
			fin.read((char*)&card, sizeof card);
			std::cout << card.card_id << std::setw(15) << card.user_name << std::setw(15) << card.department << std::setw(15) << card.type << std::endl;
			pos = cardBpTree.iter(p);
		}
		fin.close();
	}
	std::cout << std::endl;
}