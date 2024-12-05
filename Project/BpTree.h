#ifndef _B_PLUS_TREE_H
#define _B_PLUS_TREE_H

#include <vector>

#define M 4

struct KV
{
    int k;
    int v;
};

class BpTreeNode
{
public:
    bool isLeaf;                        // �Ƿ���Ҷ�ڵ�
    std::vector<int> keys;              // ��ֵ
    std::vector<BpTreeNode*> children;  // �ӽڵ�
    std::vector<int> values;            // �����Ҷ�ڵ㣬�򴢴�ֵ
    BpTreeNode* next;                   // �����Ҷ�ڵ㣬��ָ����һ��Ҷ�ڵ�
    BpTreeNode* parent;                 // ���ڵ�

    BpTreeNode(bool _isLeaf);
};

class BpTree
{
public:
    BpTree();
    BpTreeNode* findLeaf(int k);
    void insertToLeaf(int k, int v);
    BpTreeNode* getHead();
    void find(int k);
private:
    BpTreeNode* root;
    BpTreeNode* head;
    int order;
    void insertToNode(BpTreeNode* node, BpTreeNode* leaf, BpTreeNode* newLeaf, int k);
    void splitNode(BpTreeNode* node);
    void updateInnerNode(BpTreeNode* node, int it, int k);
};


#endif // !_B_PULS_TREE_H
