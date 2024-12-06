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
    /*LibrarySystem lib;
    std::vector<Book> books;*/
    //lib.clearKV();
    /*for (int i = 0; i < 2000000; i++)
    {
        Book book("C++ Primer Plus", "K12312", "Sombody", "America", i+1, i+1);
        books.push_back(book);
    }*/
    //lib.storeBooks(books);
    //lib.removeBook(10);
    //lib.show();
    //lib.queryBook(1);
    //t1 = std::chrono::system_clock::now();
    //lib.queryBook(10);
    //t2 = std::chrono::system_clock::now();
    //calTime(t1, t2);

    BpTree tree;
    auto t1 = std::chrono::system_clock::now();
    tree.findPos(-1);
    auto t2 = std::chrono::system_clock::now();
    calTime(t1, t2);
    t1 = std::chrono::system_clock::now();
    tree.insertToLeaf(2000000, 2000000);
    t2 = std::chrono::system_clock::now();
    calTime(t1, t2);
    
    /*while (std::cin >> k)
    {
        tree.insertToLeaf(k, k);
        tree.show();
    }
    std::cin.clear();
    std::cin.ignore(10, '\n');*/

    //for (int i = 1; i <= 1000000; i++)
    //{
    //    tree.insertToLeaf(i, i+100);
    //    //tree.show();
    //}

    //while (std::cin >> k)
    //{
    //    tree.deleteLeaf(k);
    //    tree.show();
    //}

    /*std::fstream treeFile;
    treeFile.open("Dataset/tree.dat", std::ios::in | std::ios::out | std::ios::binary);
    bool i = true;
    int x = 12;
    treeFile.seekp(0);
    treeFile.write((char*)&i, sizeof i);
    treeFile.seekp(sizeof i);
    treeFile.write((char*)&x, sizeof x);

    treeFile.seekg(0);
    char buffer[PAGE_SIZE];
    treeFile.read(buffer, PAGE_SIZE);

    bool is;
    int y;
    int offset = 0;
    std::memcpy(&is, buffer + offset, sizeof(bool));
    offset += sizeof(bool);
    std::memcpy(&y, buffer + offset, sizeof(int));
    std::cout << is << " " << y << std::endl;*/
}