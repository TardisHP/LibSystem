#ifndef _B_PLUS_TREE_H
#define _B_PLUS_TREE_H

#include <fstream>
#include <vector>

#define PAGE_SIZE 4096
#define MAX_LEAF 256

struct Pair
{
    long k;     // pos
    long v;     // index
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

    BpTreeNode(bool _isLeaf = false);
};

class BpTree
{
public:
    long head;
    BpTree(std::string tree, std::string node);
    ~BpTree();
    int insertToLeaf(int k, long p);   // �����ֵ��
    int deleteLeaf(int k);              // ɾ��
    long findPos(int k);                // ����
    long iter(Pair& pair);              // ����
    void show();
private:
    std::vector<long> parentStackKey;         // ���ڵ�ջ�����ڸ��¼�ֵ
    std::vector<long> parentStackNode;        // ���ڵ�ջ�����ڱ����ӽڵ�
    std::fstream treeFile;
    std::fstream treeNodeFile;
    long root;
    long maxPoint;
    BpTreeNode node;

    long findLeaf(int k);
    void serialize(long point, BpTreeNode& n);
    void deserialize(long point);
    void insertToNode(long p, long leafp, long newLeafp, int k);
    void splitNode(long p);
    void deleteNode(long p, int it, int k);
    void updateInnerNode(long p, int it, int k);
    void updateInnerNode(long p, int k);
};


#endif // !_B_PULS_TREE_H
