#include <fstream>
#include <iostream>
#include <chrono>
#include <vector>

#include "BpTree.h"
#include "LibrarySystem.h"
#include "Book.h"

//std::fstream calfile;

void calTime(std::chrono::system_clock::time_point t1, std::chrono::system_clock::time_point t2)
{
    std::chrono::duration<double, std::milli> fp_ms = t2 - t1;
    auto int_ms = std::chrono::duration_cast<std::chrono::milliseconds>(fp_ms);
    std::cout << "took " << fp_ms.count() << " ms" << std::endl;
    //calfile << fp_ms.count() << std::endl;
}

int main()
{
    //calfile.open("cal.txt", std::ios::in | std::ios::out);

    auto t1 = std::chrono::system_clock::now();
    LibrarySystem lib;
    auto t2 = std::chrono::system_clock::now();
    calTime(t1, t2);

    //t1 = std::chrono::system_clock::now();
    //lib.storeBooks("Dataset/addList_1t.dat");
    //t2 = std::chrono::system_clock::now();
    ////calTime(t1, t2);

    /*std::cout << std::endl;
    for (int i = 0; i < 50; i++)
    {
        std::vector<Book> books;
        t1 = std::chrono::system_clock::now();
        lib.queryBook(books, -2, -1);
        t2 = std::chrono::system_clock::now();
        calTime(t1, t2);
    }

    std::cout << std::endl;
    for (int i = 0; i < 50; i++)
    {
        std::vector<Book> books;
        t1 = std::chrono::system_clock::now();
        lib.queryBook(books, "-1", BY_CATEGORY);
        t2 = std::chrono::system_clock::now();
        calTime(t1, t2);
    }

    std::cout << std::endl;
    for (int i = 0; i < 50; i++)
    {
        std::vector<Book> books;
        t1 = std::chrono::system_clock::now();
        lib.queryBook(books, "qwerqwrqwrqw", BY_TITLE);
        t2 = std::chrono::system_clock::now();
        calTime(t1, t2);
    }*/

    //std::cout << std::endl;
    //for (int i = 0; i < 50; i++)
    //{
    //    std::vector<Book> books;
    //    t1 = std::chrono::system_clock::now();
    //    lib.queryBook(books, -1);
    //    t2 = std::chrono::system_clock::now();
    //    calTime(t1, t2);
    //}

    //std::cout << std::endl;
    //for (int i = 0; i < 15; i++)
    //{
    //    char str[50] = "test add";
    //    str[8] = i + '0';
    //    str[9] = '\0';
    //    Book book(str, "I231.9", "cuc", "cuc", 2003, 19.8);
    //    book.stock = 5;
    //    t1 = std::chrono::system_clock::now();
    //    lib.storeBook(book);
    //    t2 = std::chrono::system_clock::now();
    //    calTime(t1, t2);
    //}

    //std::cout << std::endl;
    //for (int i = 1; i < 6; i++)
    //{
    //    t1 = std::chrono::system_clock::now();
    //    lib.removeBook(pow(12, i));
    //    t2 = std::chrono::system_clock::now();
    //    calTime(t1, t2);
    //}

    lib.run();

    //calfile.close();
}