#define _CRT_SECURE_NO_WARNINGS

#include <string>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <chrono>
#include <random>

#include "LibrarySystem.h"
#include "Book.h"
#include "Card.h"
#include "Borrow.h"

LibrarySystem::LibrarySystem(std::string bookTreeFile, std::string bookTreeNodeFile, 
	std::string borrowTreeFile, std::string borrowTreeNodeFile,
	std::string cardTreeFile, std::string cardTreeNodeFile
	)
	:bookBpTree(bookTreeFile, bookTreeNodeFile), borrowBpTree(borrowTreeFile, borrowTreeNodeFile), cardBpTree(cardTreeFile, cardTreeNodeFile)
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
	char c;
	char str[50];
	int k, s;
	std::cout << "###        Welcome To XZL Library Management System       ###" << std::endl;
	message();
	while (std::cin >> c)
	{
		switch (c)
		{
		case 's':
		{
			Book book;
			getchar();
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
			storeBook(book);
			break;
		}
		case 'i':
			std::cout << "Enter the book id:" << std::endl;
			std::cin >> k;
			std::cout << "Enter the delta stock:" << std::endl;
			std::cin >> s;
			incBookStock(k, s);
			break;
		case 'r':
			std::cout << "Enter the book id:" << std::endl;
			std::cin >> k;
			removeBook(k);
			break;
		case 'm':
		{
			Book book;
			std::cout << "Enter the book id:" << std::endl;
			std::cin >> book.book_id;
			getchar();
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
			modifyBookInfo(book);
			break;
		}
		case 'q':
			std::cout << "-------------------------------------------------------------" << std::endl
				<< "1: by id\t\t2: by category\t\t3: by title" << std::endl
				<< "4: by publisher\t\t5: by year\t\t6: by author" << std::endl
				<< "7: by price" << std::endl
				<< "e: exit" << std::endl
				<< "-------------------------------------------------------------" << std::endl;
			std::cin >> k;
			switch (k)
			{
			case 1:
				std::cin >> k;
				queryBook(k);
				break;
			case 2:
				std::cin >> str;
				queryBook(str, 0);
				break;
			case 3:
				std::cin >> str;
				queryBook(str, 1);
				break;
			case 4:
				std::cin >> str;
				queryBook(str, 2);
				break;
			case 5:
				std::cin >> k >> s;
				queryBook(k, s);
				break;
			case 6:
				std::cin >> str;
				queryBook(str, 3);
				break;
			case 7:
			{
				float k, s;
				std::cin >> k >> s;
				queryBook(k, s);
				break;
			}
			default:
				break;
			}
			break;
		case 'b':
		{
			std::cout << "Enter the card id:" << std::endl;
			std::cin >> s;
			std::cout << "Enter the book id:" << std::endl;
			std::cin >> k;
			Borrow b(s, k);
			borrowBook(b);
			break;
		}
		case 't':
		{
			std::cout << "Enter the card id:" << std::endl;
			std::cin >> s;
			std::cout << "Enter the book id:" << std::endl;
			std::cin >> k;
			Borrow b(s, k);
			returnBook(b);
			break;
		}
		case 'h':
			showBorrowHistory(123);
			break;
		case 'c':
		{
			char na[50];
			char dp[50];
			std::cout << "Enter your name:" << std::endl;
			std::cin >> na;
			std::cout << "Enter your departure:" << std::endl;
			std::cin >> dp;
			std::cout << "Enter your identity:" << std::endl;
			std::cin >> c;
			Card card(na, dp, c);
			registerCard(card);
			break;
		}
		case 'o':
			std::cout << "Enter the card id:" << std::endl;
			std::cin >> k;
			removeCard(k);
			break;
		case 'p':
			showCards();
			break;
		case 'e':
			return;
		default:
			std::cout << "wrong param!" << std::endl;
			break;
		}
		message();
	}
}

int LibrarySystem::findSameBook(Book& _book, std::fstream& fin)
{
	std::vector<Book> books;
	std::vector<unsigned long> poses;
	unsigned long p = bookBpTree.head;
	poses = bookBpTree.iter(p);
	if (fin.is_open())
	{
		while (!poses.empty())
		{
			Book book;
			for (unsigned long pos : poses)
			{
				fin.seekg(pos);
				fin.read((char*)&book, sizeof book);
				if (book.publish_year == _book.publish_year && strcmp(book.title, _book.title) == 0 && strcmp(book.category, _book.category) == 0
					&& strcmp(book.author, _book.author) == 0 && strcmp(book.publisher, _book.publisher) == 0)
				{
					return 1;
				}
			}
			poses = bookBpTree.iter(p);
		}
	}
	return 0;
}

void LibrarySystem::storeBook(Book& book)
{
	std::fstream finout;
	finout.open(bookDatasetFile, std::ios::in | std::ios::out | std::ios::binary);
	unsigned long v;
	
	if (findSameBook(book, finout))
	{
		std::cout << "Book REPEATED!" << std::endl;
		return;
	}

	if (finout.is_open())
	{
		std::cout << "Storing Book..." << std::endl;
		// 生成id
		std::random_device rd;
		std::mt19937 engine(rd());
		std::uniform_int_distribution<int> dist(1, MAX_BOOK_NUM);
		do
		{
			finout.seekp(0, std::ios::end);
			v = finout.tellp();
			book.book_id = dist(engine);
		} while (bookBpTree.insertToLeaf(book.book_id, v) == 0);
		finout.seekp(0, std::ios::end);
		finout.write((char*)&book, sizeof book);
		finout.close();
	}
	std::cout << "Book[ " << book.book_id << " ]Stored!" << std::endl;
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

void LibrarySystem::storeBooks(std::string path)
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
}

void showInfo(Book& book)
{
	std::cout << "ID" << std::setw(20) << "category" << std::setw(20) << "title" << std::setw(15) << "author" << std::setw(15)
		<< "publisher" << std::setw(15) << "year" << std::setw(10) << "price" << std::setw(10) << "stock" << std::endl;
	std::cout << book.book_id << std::setw(20) << book.category << std::setw(20) << book.title << std::setw(15) << book.author << std::setw(15)
		<< book.publisher << std::setw(15) << book.publish_year << std::setw(10) << book.price << std::setw(10) << book.stock << std::endl;
	std::cout << std::endl;
}

void showInfo(std::vector<Book>& books)
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
	}
}

void LibrarySystem::queryBook(int k)
{
	Book book;
	unsigned long v = bookBpTree.findPos(k);
	if (v == -1)
	{
		std::cout << "Book [ " << k << " ] can NOT find!" << std::endl;
		return;
	}
	std::fstream fin;
	fin.open(bookDatasetFile, std::ios::in | std::ios::binary);
	if (fin.is_open())
	{
		fin.seekg(v);
		fin.read((char*)&book, sizeof book);
		showInfo(book);
		if (fin.eof())
			fin.clear();
		fin.close();
	}
}

void LibrarySystem::queryBook(int year_l, int year_r)
{
	std::fstream fin;
	std::vector<Book> books;
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
						books.clear();
						char c;
						std::cin >> c;
						if (c == 'q')
						{
							fin.close();
							return;
						}
					}
				}
			}
			poses = bookBpTree.iter(p);
		}
		fin.close();
	}
	showInfo(books);
}

void LibrarySystem::queryBook(float price_l, float price_r)
{
	std::fstream fin;
	std::vector<Book> books;
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
						books.clear();
						char c;
						std::cin >> c;
						if (c == 'q')
						{
							fin.close();
							return;
						}
					}
				}
			}
			poses = bookBpTree.iter(p);
		}
		fin.close();
	}
	showInfo(books);
}

void LibrarySystem::queryBook(const char* str, int type)
{
	std::fstream fin;
	std::vector<Book> books;
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
					if (std::strcmp(book.category, str) == 0)
					{
						books.push_back(book);
						num++;
						if (num % 20 == 0 && num > 0)
						{
							showInfo(books);
							books.clear();
							char c;
							std::cin >> c;
							if (c == 'q')
							{
								fin.close();
								return;
							}
						}
					}
					break;
				case 1:
					if (std::strstr(book.title, str))
					{
						books.push_back(book);
						num++;
						if (num % 20 == 0 && num > 0)
						{
							showInfo(books);
							books.clear();
							char c;
							std::cin >> c;
							if (c == 'q')
							{
								fin.close();
								return;
							}
						}
					}
					break;
				case 2:
					if (std::strstr(book.publisher, str))
					{
						books.push_back(book);
						num++;
						if (num % 20 == 0 && num > 0)
						{
							showInfo(books);
							books.clear();
							char c;
							std::cin >> c;
							if (c == 'q')
							{
								fin.close();
								return;
							}
						}
					}
					break;
				case 3:
					if (std::strstr(book.author, str))
					{
						books.push_back(book);
						num++;
						if (num % 20 == 0 && num > 0)
						{
							showInfo(books);
							books.clear();
							char c;
							std::cin >> c;
							if (c == 'q')
							{
								fin.close();
								return;
							}
						}
					}
					break;
				default:
					break;
				}
			}
			poses = bookBpTree.iter(p);
		}
		fin.close();
	}
	showInfo(books);
}

void LibrarySystem::modifyBookInfo(Book _book)
{
	unsigned long pos = bookBpTree.findPos(_book.book_id);
	if (pos == -1)
	{
		std::cout << "can NOT find the book" << std::endl;
		return;
	}

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
	std::cout << "Book[ " << book.book_id << " ] has been modified" << std::endl;
}

void LibrarySystem::removeBook(int k)
{
	if (bookBpTree.deleteLeaf(k))
		std::cout << "Book[ " << k << " ] has been deleted" << std::endl;
	else
		std::cout << "Book[ " << k << " ] delete FAILED!" << std::endl;
}

int LibrarySystem::incBookStock(int k, int deltaStock)
{
	Book book;
	unsigned long pos = bookBpTree.findPos(k);
	if (pos == -1)
	{
		std::cout << "Book [ " << k << " ] can NOT find!" << std::endl;
		return 0;
	}
	std::fstream finout;
	finout.open(bookDatasetFile, std::ios::in | std::ios::out | std::ios::binary);
	if (finout.is_open())
	{
		finout.seekg(pos);
		finout.read((char*)&book, sizeof book);
		book.stock += deltaStock;
		if (book.stock < 0)
		{
			std::cout << "Book [ " << k << " ] is out of stock!" << std::endl;
			finout.close();
			return 0;
		}
		finout.seekp(pos);
		finout.write((char*)&book, sizeof book);
		finout.close();
	}
	std::cout << "Book[ " << book.book_id << " " << book.title << " ]'s stock has been changed to " << book.stock << std::endl;
	return 1;
}

void LibrarySystem::borrowBook(Borrow& borrow)
{
	if (cardBpTree.findPos(borrow.card_id) == -1)
	{
		std::cout << "Card is illegal!" << std::endl;
		return;
	}

	unsigned long pos;
	std::fstream finout;
	finout.open(borrowDatasetFile, std::ios::in | std::ios::out | std::ios::binary);
	if (finout.is_open())
	{
		if (borrowBpTree.findPos(borrow.borrow_id) != -1)
		{
			std::cout << "Book[ " << borrow.book_id << " ] has been borrowed by " << borrow.card_id << std::endl;
			finout.close();
			return;
		}
		// 检查库存
		if (incBookStock(borrow.book_id, -1) == 0)
		{
			std::cout << "Borrow FAILED!"<< std::endl;
			finout.close();
			return;
		}
		finout.seekp(0, std::ios::end);
		pos = finout.tellp();
		// std::cout << "Borrowing Book..." << std::endl;
		// 获取时间
		auto now = std::chrono::system_clock::now();
		std::time_t now_c = std::chrono::system_clock::to_time_t(now);
		borrow.borrow_date = now_c;
		finout.write((char*)&borrow, sizeof borrow);
		borrowBpTree.insertToLeaf(borrow.borrow_id, pos);
		finout.close();
	}
	std::cout << "Borrow SUCCESSED!" << std::endl;
}

void LibrarySystem::returnBook(Borrow& borrow)
{
	if (cardBpTree.findPos(borrow.card_id) == -1)
	{
		std::cout << "Card is illegal!" << std::endl;
		return;
	}

	unsigned long pos = borrowBpTree.findPos(borrow.borrow_id);
	std::fstream finout;
	finout.open(borrowDatasetFile, std::ios::in | std::ios::out | std::ios::binary);
	if (finout.is_open())
	{
		if (pos == -1)
		{
			std::cout << "Return FAILED!" << std::endl;
			finout.close();
			return;
		}
		Borrow btmp;
		finout.seekg(pos);
		finout.read((char*)&btmp, sizeof btmp);
		// std::cout << "Borrowing Book..." << std::endl;
		if (btmp.return_date != 0)
		{
			std::cout << "Has been returned!" << std::endl;
			finout.close();
			return;
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

void LibrarySystem::registerCard(Card& card)
{
	if (card.card_id != -1)
	{
		std::cout << "Card has been registed!" << std::endl;
		return;
	}
	if (card.user_name[0] == '\0')
	{
		std::cout << "Card has problem" << std::endl;
		return;
	}
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
				std::cout << "Card has been registered!" << std::endl;
				finout.close();
				return;
			}
			pos = borrowBpTree.iter(p);
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
	std::cout << "Card register SUCCESS!" << std::endl;
}

void LibrarySystem::removeCard(int cardId)
{
	if (cardBpTree.deleteLeaf(cardId))
		std::cout << "Card[ " << cardId << " ] has been removed" << std::endl;
	else
		std::cout << "Card[ " << cardId << " ] remove FAILED!" << std::endl;
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