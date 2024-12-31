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
    bool isLeaf;                        // 是否是叶节点
    int keycount;                       // 键值个数
    std::vector<int> keys;              // 键值
    std::vector<unsigned long> pointers;         // 子节点位置，如果是叶节点，则储存文件位置
    unsigned long pre;                           // 如果是叶节点，则指向上一个叶节点
    unsigned long next;                          // 如果是叶节点，则指向下一个叶节点

    BpTreeNode(bool _isLeaf = false);
};

class BpTree
{
public:
    unsigned long head;
    BpTree(std::string tree, std::string node);
    ~BpTree();
    int insertToLeaf(int k, unsigned long p);               // 插入键值对
    int deleteLeaf(int k);                                  // 删除
    unsigned long findPos(int k);                           // 查找
    unsigned long iter(Pair& pair);                         // 遍历
    std::vector<unsigned long>& iter(unsigned long& p);     // 遍历
private:
    std::vector<unsigned long> parentStackKey;         // 父节点栈，用于更新键值
    std::vector<unsigned long> parentStackNode;        // 父节点栈，用于遍历子节点
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
