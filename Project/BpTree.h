#ifndef _B_PLUS_TREE_H
#define _B_PLUS_TREE_H

#include <fstream>
#include <vector>

#define PAGE_SIZE 4096
#define MAX_LEAF 400

struct KV
{
    int k;
    std::streampos v;
};

class BpTreeNode
{
public:
    bool isLeaf;                        // �Ƿ���Ҷ�ڵ�
    int keycount;                       // ��ֵ����
    std::vector<int> keys;              // ��ֵ
    std::vector<long> pointers;         // �ӽڵ�λ�ã������Ҷ�ڵ㣬�򴢴��ļ�λ��
    long pre;                           // �����Ҷ�ڵ㣬��ָ����һ��Ҷ�ڵ�
    long next;                          // �����Ҷ�ڵ㣬��ָ����һ��Ҷ�ڵ�
    long parent;                        // ���ڵ�

    BpTreeNode(bool _isLeaf);
    //~BpTreeNode();
};

class BpTree
{
public:
    BpTree();
    ~BpTree();
    long findLeaf(int k);
    void insertToLeaf(int k, long v);
    void deleteLeaf(int k);
    long getHead();
    long findPos(int k);
    void show();
private:
    std::fstream treeFile;
    std::fstream treeNodeFile;
    long root;
    long head;
    long maxPoint;
    BpTreeNode node;

    void serialize(long point, BpTreeNode& n);
    void deserialize(long point);
    void insertToNode(long p, long leafp, long newLeafp, int k);
    void splitNode(long p);
    void deleteNode(long node, int it, int k);
    void updateInnerNode(long p, int it, int k);
    void updateInnerNode(long node, int k);
};


#endif // !_B_PULS_TREE_H
