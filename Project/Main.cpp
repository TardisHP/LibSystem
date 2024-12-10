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
    std::cout << "took " << fp_ms.count() << " ms" << std::endl;
}

int main()
{
    auto t1 = std::chrono::system_clock::now();
    LibrarySystem lib;
    auto t2 = std::chrono::system_clock::now();
    calTime(t1, t2);

    //t1 = std::chrono::system_clock::now();
    //lib.storeBooks("Dataset/addList copy.dat");
    //t2 = std::chrono::system_clock::now();
    //calTime(t1, t2);

    //t1 = std::chrono::system_clock::now();
    //lib.queryBook(-2, -1);
    //t2 = std::chrono::system_clock::now();
    //calTime(t1, t2);

    //t1 = std::chrono::system_clock::now();
    //lib.queryBook("-1", 0);
    //t2 = std::chrono::system_clock::now();
    //calTime(t1, t2);

    //t1 = std::chrono::system_clock::now();
    //lib.queryBook("-1", 1);
    //t2 = std::chrono::system_clock::now();
    //calTime(t1, t2);

    //t1 = std::chrono::system_clock::now();
    //lib.queryBook(-1);
    //t2 = std::chrono::system_clock::now();
    //calTime(t1, t2);

    //Book book("chinese history", "I231.9", "cuc", "cuc", 2003, 19.8);
    //book.stock = 5;
    //t1 = std::chrono::system_clock::now();
    //lib.storeBook(book);
    //t2 = std::chrono::system_clock::now();
    //calTime(t1, t2);

    //t1 = std::chrono::system_clock::now();
    //lib.removeBook(52100);
    //t2 = std::chrono::system_clock::now();
    //calTime(t1, t2);

    lib.run();
}