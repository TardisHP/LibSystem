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
    bool isLeaf;                        // 是否是叶节点
    std::vector<int> keys;              // 键值
    std::vector<BpTreeNode*> children;  // 子节点
    std::vector<std::streampos> values; // 如果是叶节点，则储存文件位置
    BpTreeNode* next;                   // 如果是叶节点，则指向下一个叶节点
    BpTreeNode* parent;                 // 父节点

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
