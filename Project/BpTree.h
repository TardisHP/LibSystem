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
    bool isLeaf;                        // 是否是叶节点
    std::vector<int> keys;              // 键值
    std::vector<BpTreeNode*> children;  // 子节点
    std::vector<int> values;            // 如果是叶节点，则储存值
    BpTreeNode* next;                   // 如果是叶节点，则指向下一个叶节点
    BpTreeNode* parent;                 // 父节点

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
