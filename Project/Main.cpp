#include <iostream>
#include <chrono>
#include <vector>

#include "BpTree.h"
#include "LibrarySystem.h"
#include "Book.h"

int main()
{
    /*auto t1 = std::chrono::system_clock::now();*/
    //tree.find(-1);
    LibrarySystem lib;
    std::vector<Book> books;
    /*auto t2 = std::chrono::system_clock::now();
    std::chrono::duration<double, std::milli> fp_ms = t2 - t1;
    auto int_ms = std::chrono::duration_cast<std::chrono::milliseconds>(fp_ms);
    std::cout << "took " << fp_ms.count() << " ms\n";*/
    //lib.clearKV();
    /*for (int i = 0; i < 1000000; i++)
    {
        Book book("C++ Primer Plus", "K12312", "Sombody", "America", i+1, i+1);
        books.push_back(book);
    }
    lib.storeBooks(books);*/

    //lib.show();
    lib.queryBook(238953);
}