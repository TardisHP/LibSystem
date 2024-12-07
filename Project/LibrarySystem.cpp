#include <string>
#include <iostream>
#include <fstream>

#include "LibrarySystem.h"
#include "Book.h"
#include "Card.h"
#include "Borrow.h"

LibrarySystem::LibrarySystem(std::string bookTreeFile, std::string bookTreeNodeFile)
: bookBpTree(bookTreeFile, bookTreeNodeFile)
{
	bookDatasetFile = "Dataset/bookDataset.dat";
	keyMax = 0;
}

void LibrarySystem::run()
{
	char c;
	int k, s;
	std::cout << "### Welcome To XZL Library Management System ###" << std::endl;
	std::cout << "------------------------------------------------" << std::endl
		<< "s: store a book\t\ti: change inc\t\tr: remove a book" << std::endl
		<< "q: query books\t\te: exit" << std::endl
		<< "------------------------------------------------" << std::endl;
	while (std::cin >> c)
	{
		switch (c)
		{
		case 's':
			std::cout << "stored" << std::endl;
			break;
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
		case 'q':
			std::cout << "Enter the book id:" << std::endl;
			std::cin >> k;
			queryBook(k);
			break;
		case 'e':
			return;
		default:
			std::cout << "wrong param!" << std::endl;
			break;
		}
		std::cout << "------------------------------------------------" << std::endl
			<< "s: store a book\t\ti: change inc\t\tr: remove a book" << std::endl
			<< "q: query books\t\te: exit" << std::endl
			<< "------------------------------------------------" << std::endl;
	}
}

void LibrarySystem::storeBook(Book& book)
{
	long v;
	std::fstream finout;
	finout.open(bookDatasetFile, std::ios::in | std::ios::out | std::ios::binary);
	if (finout.is_open())
	{
		finout.seekp(0, std::ios::end);
		v = finout.tellp();
		// std::cout << "Storing Book..." << std::endl;
		book.book_id = ++keyMax;
		finout.write((char*)&book, sizeof book);
		finout.close();
		bookBpTree.insertToLeaf(book.book_id, v);
	}
}

void LibrarySystem::storeBooks(std::vector<Book>& books)
{
	long v;
	long offset = sizeof Book;
	std::fstream finout;
	std::cout << "Start Stroing..." << std::endl;
	finout.open(bookDatasetFile, std::ios::in | std::ios::out | std::ios::binary);
	if (finout.is_open())
	{
		int num = 0;
		int iter = books.size() / 10;
		finout.seekp(0, std::ios::end);
		for (auto& book : books)
		{
			v = finout.tellp();
			book.book_id = ++keyMax;
			finout.write((char*)&book, sizeof book);
			bookBpTree.insertToLeaf(book.book_id, v);
			finout.seekp(v + offset);
			if (++num % iter == 0)
				std::cout << "# " << num << " has benn finished..." << std::endl;
		}
		finout.close();
	}
	std::cout << "Store finished!" << std::endl;
}

void LibrarySystem::queryBook(int k)
{
	Book book;
	long v = bookBpTree.findPos(k);
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
		std::cout << book.book_id << " " << book.category << " " << book.title << " " << book.author << " " << book.publisher << " " << book.publish_year << " " << book.price << " " << " " << book.stock << std::endl;
		if (fin.eof())
			fin.clear();
		fin.close();
	}
}

void LibrarySystem::queryBook(int k, int y)
{
	/*bool isFind = false;
	Book book;
	long p = bpTree.getHead();
	std::fstream fin;
	fin.open(datasetFile, std::ios::in | std::ios::binary);
	if (fin.is_open())
	{
		while (p)
		{
			for (int i = 0; i < p->keys.size(); i++)
			{
				fin.seekg(p->values[i]);
				fin.read((char*)&book, sizeof book);
				if (book.publish_year == y)
				{
					std::cout << book.book_id << " " << book.category << " " << book.title << " " << book.author << " " << book.publisher << " " << book.publish_year << " " << book.price << " " << " " << book.stock << std::endl;
					isFind = true;
				}
			}
			p = p->next;
		}
		if (fin.eof())
			fin.clear();
		fin.close();
	}
	if (!isFind)
		std::cout << "cant find" << std::endl;*/
}

void LibrarySystem::removeBook(int k)
{
	if (bookBpTree.deleteLeaf(k))
		std::cout << "Book[ " << k << " ] has been deleted" << std::endl;
	else
		std::cout << "Book[ " << k << " ] delete FAILED!" << std::endl;
}

void LibrarySystem::incBookStock(int k, int deltaStock)
{
	Book book;
	long pos = bookBpTree.findPos(k);
	if (pos == -1)
	{
		std::cout << "Book [ " << k << " ] can NOT find!" << std::endl;
		return;
	}
	std::fstream finout;
	finout.open(bookDatasetFile, std::ios::in | std::ios::out | std::ios::binary);
	if (finout.is_open())
	{
		finout.seekg(pos);
		finout.seekp(pos);
		finout.read((char*)&book, sizeof book);
		book.stock += deltaStock;
		finout.write((char*)&book, sizeof book);
		finout.close();
	}
	std::cout << "Book[ " << book.book_id << " " << book.title << " ]'s stock has been changed to " << book.stock << std::endl;
}