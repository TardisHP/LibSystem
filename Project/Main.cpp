#include <fstream>
#include <iostream>
#include <chrono>
#include <vector>
#include <Windows.h>

#include "BpTree.h"
#include "LibrarySystem.h"
#include "Book.h"

LibrarySystem* lib;

void calTime(std::chrono::system_clock::time_point t1, std::chrono::system_clock::time_point t2)
{
    std::chrono::duration<double, std::milli> fp_ms = t2 - t1;
    auto int_ms = std::chrono::duration_cast<std::chrono::milliseconds>(fp_ms);
    std::cout << "took " << fp_ms.count() << " ms" << std::endl;
}

BOOL WINAPI ConsoleHandler(DWORD ctrlType) {
    if (ctrlType == CTRL_CLOSE_EVENT) {
        // 在此处执行必要的清理操作
        delete lib;
        return TRUE; // 返回TRUE可尝试阻止默认行为，但通常系统仍会关闭窗口
    }
    return FALSE;
}

int main()
{
    SetConsoleCtrlHandler(ConsoleHandler, TRUE);

    auto t1 = std::chrono::system_clock::now();
    lib = new LibrarySystem;
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

    lib->run();
}