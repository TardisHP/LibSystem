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
    bool isLeaf;                        // 是否是叶节点
    int keycount;                       // 键值个数
    std::vector<int> keys;              // 键值
    std::vector<long> pointers;         // 子节点位置，如果是叶节点，则储存文件位置
    long pre;                           // 如果是叶节点，则指向上一个叶节点
    long next;                          // 如果是叶节点，则指向下一个叶节点

    BpTreeNode(bool _isLeaf = false);
};

class BpTree
{
public:
    long head;
    BpTree(std::string tree, std::string node);
    ~BpTree();
    int insertToLeaf(int k, long p);   // 插入键值对
    int deleteLeaf(int k);              // 删除
    long findPos(int k);                // 查找
    long iter(Pair& pair);              // 遍历
    void show();
private:
    std::vector<long> parentStackKey;         // 父节点栈，用于更新键值
    std::vector<long> parentStackNode;        // 父节点栈，用于遍历子节点
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
