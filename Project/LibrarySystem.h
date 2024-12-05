#ifndef _LIBRARY_SYSTEM_H
#define _LIBRARY_SYSTEM_H

#include <string>

#include "BpTree.h"

class LibrarySystem
{
public:
	LibrarySystem();
	void storeBook();
	void storeBooks();
	void clearKV();
	void show();
private:
	std::string kvFile;
	std::string datasetFile;
	BpTree bpTree;

	void createBpTreeFromFile();
	void saveBpTreeToFile();
};

#endif // !_LIBRARY_SYSTEM_H
