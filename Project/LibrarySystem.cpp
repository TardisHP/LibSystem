#include <string>
#include <iostream>
#include <fstream>

#include "LibrarySystem.h"

LibrarySystem::LibrarySystem()
{
	kvFile = "Dataset/kvlist.dat";
	datasetFile = "Dataset/data.dat";
	createBpTreeFromFile();
}

void LibrarySystem::storeBook()
{
	int k, v;
	std::cin >> k >> v;
	bpTree.insertToLeaf(k, v);
	saveBpTreeToFile();
}

void LibrarySystem::storeBooks()
{
	int k, v;
	for (int i = 0; i < 999999; i++)
	{
		bpTree.insertToLeaf(i, i+1);
	}
	saveBpTreeToFile();
}

void LibrarySystem::show()
{
	BpTreeNode* p = bpTree.getHead();
	while (p)
	{
		for (int i = 0; i < p->keys.size(); i++)
		{
			std::cout << p->keys[i] << " " << p->values[i] << std::endl;
		}
		p = p->next;
	}
}

void LibrarySystem::clearKV()
{
	std::fstream finout;
	finout.open(kvFile, std::ios::out | std::ios::trunc | std::ios::binary);
	finout.close();
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
		}
		if (fin.eof())
			fin.clear();
	}
	fin.close();
	std::cout << "Build Done" << std::endl;
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
	}
	fout.close();
}