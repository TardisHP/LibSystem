#ifndef _B_PLUS_TREE_H
#define _B_PLUS_TREE_H

#include <fstream>
#include <vector>

#define PAGE_SIZE 4096     // 4K
#define MAX_LEAF 256

struct Pair
{
    unsigned long k;     // pos
    unsigned long v;     // index
};

class BpTreeNode
{
public:
    bool isLeaf;                        // �Ƿ���Ҷ�ڵ�
    int keycount;                       // ��ֵ����
    std::vector<int> keys;              // ��ֵ
    std::vector<unsigned long> pointers;         // �ӽڵ�λ�ã������Ҷ�ڵ㣬�򴢴��ļ�λ��
    unsigned long pre;                           // �����Ҷ�ڵ㣬��ָ����һ��Ҷ�ڵ�
    unsigned long next;                          // �����Ҷ�ڵ㣬��ָ����һ��Ҷ�ڵ�

    BpTreeNode(bool _isLeaf = false);
};

class BpTree
{
public:
    unsigned long head;
    BpTree(std::string tree, std::string node);
    ~BpTree();
    int insertToLeaf(int k, unsigned long p);               // �����ֵ��
    int deleteLeaf(int k);                                  // ɾ��
    unsigned long findPos(int k);                           // ����
    unsigned long iter(Pair& pair);                         // ����
    std::vector<unsigned long>& iter(unsigned long& p);     // ����
private:
    std::vector<unsigned long> parentStackKey;         // ���ڵ�ջ�����ڸ��¼�ֵ
    std::vector<unsigned long> parentStackNode;        // ���ڵ�ջ�����ڱ����ӽڵ�
    std::fstream treeFile;
    std::fstream treeNodeFile;
    unsigned long root;
    unsigned long maxPoint;
    BpTreeNode node;

    unsigned long findLeaf(int k);
    void serialize(unsigned long point, BpTreeNode& n);
    void deserialize(unsigned long point);
    void insertToNode(unsigned long p, unsigned long leafp, unsigned long newLeafp, int k);
    void splitNode(unsigned long p);
    void deleteNode(unsigned long p, int it, int k);
    void updateInnerNode(unsigned long p, int it, int k);
    void updateInnerNode(unsigned long p, int k);
};


#endif // !_B_PULS_TREE_H
