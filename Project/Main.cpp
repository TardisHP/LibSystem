#include <iostream>
#include <chrono>
#include <vector>

#include "BpTree.h"
#include "LibrarySystem.h"
#include "Book.h"

void calTime(std::chrono::system_clock::time_point t1, std::chrono::system_clock::time_point t2)
{
    std::vector<Book> books;
    std::chrono::duration<double, std::milli> fp_ms = t2 - t1;
    auto int_ms = std::chrono::duration_cast<std::chrono::milliseconds>(fp_ms);
    std::cout << "took " << fp_ms.count() << " ms\n";
}

int main()
{
    //LibrarySystem lib;
    //lib.clearKV();
    /*for (int i = 0; i < 1000000; i++)
    {
        Book book("C++ Primer Plus", "K12312", "Sombody", "America", i+1, i+1);
        books.push_back(book);
    }
    lib.storeBooks(books);*/
    //lib.removeBook(10);
    //lib.show();
    //auto t1 = std::chrono::system_clock::now();
    //lib.queryBook(1);
    //auto t2 = std::chrono::system_clock::now();
    //calTime(t1, t2);
    //t1 = std::chrono::system_clock::now();
    //lib.queryBook(10);
    //t2 = std::chrono::system_clock::now();
    //calTime(t1, t2);

    BpTree tree;
    int k;
    
    /*while (std::cin >> k)
    {
        tree.insertToLeaf(k, k);
        tree.show();
    }
    std::cin.clear();
    std::cin.ignore(10, '\n');*/

    for (int i = 0; i < 50; i++)
    {
        tree.insertToLeaf(i, i);
    }
    tree.show();

    while (std::cin >> k)
    {
        tree.deleteLeaf(k);
        tree.show();
    }

}