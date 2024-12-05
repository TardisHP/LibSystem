#ifndef _B_PLUS_TREE_H
#define _B_PLUS_TREE_H

#include <fstream>
#include <vector>

#define M 4

struct KV
{
    int k;
    std::streampos v;
};

class BpTreeNode
{
public:
    bool isLeaf;                        // �Ƿ���Ҷ�ڵ�
    std::vector<int> keys;              // ��ֵ
    std::vector<BpTreeNode*> children;  // �ӽڵ�
    std::vector<std::streampos> values; // �����Ҷ�ڵ㣬�򴢴��ļ�λ��
    BpTreeNode* next;                   // �����Ҷ�ڵ㣬��ָ����һ��Ҷ�ڵ�
    BpTreeNode* parent;                 // ���ڵ�

    BpTreeNode(bool _isLeaf);
};

class BpTree
{
public:
    BpTree();
    BpTreeNode* findLeaf(int k);
    void insertToLeaf(int k, std::streampos v);
    BpTreeNode* getHead();
    std::streampos findPos(int k);
private:
    BpTreeNode* root;
    BpTreeNode* head;
    int order;
    void insertToNode(BpTreeNode* node, BpTreeNode* leaf, BpTreeNode* newLeaf, int k);
    void splitNode(BpTreeNode* node);
    void updateInnerNode(BpTreeNode* node, int it, int k);
};


#endif // !_B_PULS_TREE_H
