#include <fstream>
#include <iostream>
#include <chrono>
#include <vector>

#include "BpTree.h"
#include "LibrarySystem.h"
#include "Book.h"


void calTime(std::chrono::system_clock::time_point t1, std::chrono::system_clock::time_point t2)
{
    std::chrono::duration<double, std::milli> fp_ms = t2 - t1;
    auto int_ms = std::chrono::duration_cast<std::chrono::milliseconds>(fp_ms);
    std::cout << "took " << fp_ms.count() << " ms\n";
}

int main()
{
    LibrarySystem lib;

    /*std::vector<Book> books;
    for (int i = 1; i <= 1000; i++)
    {
        Book book("C++ Primer Plus", "K12312", "Sombody", "America", i, i);
        books.push_back(book);
    }
    auto t1 = std::chrono::system_clock::now();
    lib.storeBooks(books);
    auto t2 = std::chrono::system_clock::now();
    calTime(t1, t2);*/
    //lib.storeBooks("Dataset/addList copy.txt");

    //auto t1 = std::chrono::system_clock::now();
    //lib.queryBook("C++", 1);
    //auto t2 = std::chrono::system_clock::now();
    //calTime(t1, t2);

    lib.run();

    
}