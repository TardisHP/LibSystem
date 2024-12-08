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

    //auto t1 = std::chrono::system_clock::now();
    //lib.storeBooks("Dataset/addList copy.dat");
    //auto t2 = std::chrono::system_clock::now();
    //calTime(t1, t2);

    //auto t1 = std::chrono::system_clock::now();
    //lib.queryBook(-2, -1);
    //auto t2 = std::chrono::system_clock::now();
    //calTime(t1, t2);

    lib.run();
}