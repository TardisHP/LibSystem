#include <string>
#include <iostream>
#include <fstream>

#include "LibrarySystem.h"
#include "Book.h"
#include "Card.h"
#include "Borrow.h"

LibrarySystem::LibrarySystem()
{
	kvFile = "Dataset/kvlist.dat";
	datasetFile = "Dataset/data.dat";
	keyMax = 0;
	createBpTreeFromFile();
}

void LibrarySystem::storeBook(Book& book)
{
	std::streampos v;
	std::fstream finout;
	finout.open(datasetFile, std::ios::in | std::ios::out | std::ios::binary);
	if (finout.is_open())
	{
		finout.seekp(0, std::ios::end);
		v = finout.tellp();
		// std::cout << "Storing Book..." << std::endl;
		book.book_id = ++keyMax;
		finout.write((char*)&book, sizeof book);
		finout.close();
		bpTree.insertToLeaf(book.book_id, v);
		saveBpTreeToFile();
	}
}

void LibrarySystem::storeBooks(std::vector<Book>& books)
{
	std::streampos v;
	std::streampos offset = sizeof Book;
	std::fstream finout;
	finout.open(datasetFile, std::ios::in | std::ios::out | std::ios::binary); if (finout.is_open())
	{
		finout.seekp(0, std::ios::end);
		for (auto book : books)
		{
			v = finout.tellp();
			// std::cout << "Storing Book..." << std::endl;
			book.book_id = ++keyMax;
			finout.write((char*)&book, sizeof book);
			bpTree.insertToLeaf(book.book_id, v);
			finout.seekp(v + offset);
		}
		finout.close();
		saveBpTreeToFile();
	}
}

void LibrarySystem::show()
{
	Book book;
	std::fstream fin;
	fin.open(datasetFile, std::ios::in | std::ios::binary);
	if (fin.is_open())
	{
		BpTreeNode* p = bpTree.getHead();
		while (p)
		{
			for (int i = 0; i < p->keys.size(); i++)
			{
				fin.seekg(p->values[i]);
				fin.read((char*)&book, sizeof book);
				std::cout << book.book_id << " " << book.category << " " << book.title << " " << book.author << " " << book.publisher << " " << book.publish_year << " " << book.price << " " << " " << book.stock << std::endl;
			}
			p = p->next;
		}
		if (fin.eof())
			fin.clear();
		fin.close();
	}
}

void LibrarySystem::clearKV()
{
	std::fstream finout;
	finout.open(kvFile, std::ios::out | std::ios::trunc | std::ios::binary);
	finout.close();
	finout.open(datasetFile, std::ios::out | std::ios::trunc | std::ios::binary);
	finout.close();
}

void LibrarySystem::queryBook(int k)
{
	Book book;
	std::streampos v = bpTree.findPos(k);
	if (v == -1)
	{
		std::cout << "cant find" << std::endl;
		return;
	}
	std::fstream fin;
	fin.open(datasetFile, std::ios::in | std::ios::binary);
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

void LibrarySystem::createBpTreeFromFile()
{
	KV kv;
	std::fstream fin;
	fin.open(kvFile, std::ios::in | std::ios::binary);
	if (fin.is_open())
	{
		fin.seekg(0);
		std::cout << "Building Tree..." << std::endl;
		while (fin.read((char*)&kv, sizeof kv))
		{
			bpTree.insertToLeaf(kv.k, kv.v);
			keyMax = kv.k;
		}
		if (fin.eof())
			fin.clear();
		fin.close();
		std::cout << "Build Done" << std::endl;
	}
}

void LibrarySystem::saveBpTreeToFile()
{
	KV kv;
	std::fstream fout;
	fout.open(kvFile, std::ios::out | std::ios::binary);
	if (fout.is_open())
	{
		BpTreeNode* p = bpTree.getHead();
		long rec = 0;
		while (p)
		{
			for (int i = 0; i < p->keys.size(); i++)
			{
				kv.k = p->keys[i];
				kv.v = p->values[i];
				fout.seekp(rec * sizeof kv);
				fout.write((char*)&kv, sizeof kv);
				rec++;
			}
			p = p->next;
		}
		fout.close();
	}
}