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
    //std::cout << fp_ms.count() << std::endl;
}

BOOL WINAPI ConsoleHandler(DWORD ctrlType) {
    if (ctrlType == CTRL_CLOSE_EVENT) {
        // �ڴ˴�ִ�б�Ҫ���������
        delete lib;
        return TRUE; // ����TRUE�ɳ�����ֹĬ����Ϊ����ͨ��ϵͳ�Ի�رմ���
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

    //int offset = 0;
    //offset += sizeof(bool);
    //offset += sizeof(int);
    //for (int i = 0; i < MAX_LEAF; i++)
    //{
    //    offset += sizeof(int);
    //}
    //for (int i = 0; i < MAX_LEAF; i++)
    //{
    //    offset += sizeof(unsigned long);
    //}
    //offset += sizeof(unsigned long);
    //offset += sizeof(unsigned long);
    //std::cout << offset << std::endl;

    //class BookTmp
    //{
    //public:
    //    int book_id;				//���
    //    char title[MAX_LEN];		//����
    //    char category[MAX_LEN];		//���
    //    char author[MAX_LEN];		//����
    //    char publisher[MAX_LEN];	//������
    //    int publish_year;			//���
    //    float price;				//�۸�
    //    int stock;					//ʣ����
    //};
    //t1 = std::chrono::system_clock::now();
    //BookTmp tmp;
    //std::fstream file;
    //file.open("test.dat", std::ios::in | std::ios::binary);
    //unsigned long offset = 0;
    //for (int i = 0; i < 10000000; i++)
    //{
    //    file.seekg(offset);
    //    file.read((char*)&tmp, sizeof tmp);
    //    if (std::strstr(tmp.title, "qwerqwrqwrqw") != nullptr)
    //        ;
    //    offset += sizeof tmp;
    //}
    //t2 = std::chrono::system_clock::now();
    //calTime(t1, t2);

    //t1 = std::chrono::system_clock::now();
    //lib->storeBooks("Dataset/addList_1t.dat");
    //t2 = std::chrono::system_clock::now();
    //calTime(t1, t2);

    //std::cout << std::endl;
    //for (int i = 0; i < 50; i++)
    //{
    //    std::vector<Book> books;
    //    t1 = std::chrono::system_clock::now();
    //    lib->queryBook(books, -2, -1);
    //    t2 = std::chrono::system_clock::now();
    //    calTime(t1, t2);
    //}
    //std::cout << std::endl;
    //for (int i = 0; i < 50; i++)
    //{
    //    std::vector<Book> books;
    //    t1 = std::chrono::system_clock::now();
    //    lib->queryBook(books, "-1", BY_CATEGORY);
    //    t2 = std::chrono::system_clock::now();
    //    calTime(t1, t2);
    //}
    //std::cout << std::endl;
    //for (int i = 0; i < 50; i++)
    //{
    //    std::vector<Book> books;
    //    t1 = std::chrono::system_clock::now();
    //    lib->queryBook(books, "qwerqwrqwrqw", BY_TITLE);
    //    t2 = std::chrono::system_clock::now();
    //    calTime(t1, t2);
    //}
    //std::cout << std::endl;
    //for (int i = 0; i < 50; i++)
    //{
    //    std::vector<Book> books;
    //    t1 = std::chrono::system_clock::now();
    //    lib->queryBook(books, -1);
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
    delete lib;
}