#include <iostream>
#include <chrono>

#include "BpTree.h"
#include "LibrarySystem.h"

int main()
{
    //BpTree tree;
    //for (int i = 0; i <= 10; i++)
    //{
    //    tree.insertToLeaf(i, i + 1);
    //}
    //tree.insertToLeaf(2, 12);
    auto t1 = std::chrono::system_clock::now();
    //tree.find(-1);
    LibrarySystem lib;
    auto t2 = std::chrono::system_clock::now();
    std::chrono::duration<double, std::milli> fp_ms = t2 - t1;
    auto int_ms = std::chrono::duration_cast<std::chrono::milliseconds>(fp_ms);
    std::cout << "took " << fp_ms.count() << " ms\n";
    //lib.clearKV();
    //lib.storeBooks();
    //lib.show();
}