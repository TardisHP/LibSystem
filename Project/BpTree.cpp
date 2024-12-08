#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "BpTree.h"

BpTreeNode::BpTreeNode(bool _isLeaf) : isLeaf(_isLeaf), keycount(0), pre(-1), next(-1) {}



BpTree::BpTree(std::string tree, std::string node) : root(-1), head(-1), maxPoint(0)
{
    treeFile.open(tree, std::ios::in | std::ios::out | std::ios::binary | std::ios::ate);
    treeNodeFile.open(node, std::ios::in | std::ios::out | std::ios::binary);
    if (0 != treeFile.tellg())
    {
        treeFile.seekg(0);
        char buffer[PAGE_SIZE] = "";
        treeFile.read(buffer, PAGE_SIZE);
        int offset = 0;
        std::memcpy((char*)&root, buffer + offset, sizeof(unsigned long));
        offset += sizeof(unsigned long);
        std::memcpy((char*)&head, buffer + offset, sizeof(unsigned long));
        offset += sizeof(unsigned long);
        std::memcpy((char*)&maxPoint, buffer + offset, sizeof(unsigned long));
    }
}

BpTree::~BpTree()
{
    treeFile.seekp(0);
    char buffer[PAGE_SIZE] = "";
    int offset = 0;
    std::memcpy(buffer + offset, (char*)&root, sizeof(unsigned long));
    offset += sizeof(unsigned long);
    std::memcpy(buffer + offset, (char*)&head, sizeof(unsigned long));
    offset += sizeof(unsigned long);
    std::memcpy(buffer + offset, (char*)&maxPoint, sizeof(unsigned long));
    treeFile.write(buffer, PAGE_SIZE);
    treeNodeFile.close();
    treeFile.close(); 
}

// 将node序列化加入到文件中
void BpTree::serialize(unsigned long point, BpTreeNode& _node)
{
    treeNodeFile.seekp(point * PAGE_SIZE);
    char buffer[PAGE_SIZE] = "";
    int offset = 0;
    std::memcpy(buffer + offset, (char*)&_node.isLeaf, sizeof(bool));
    offset += sizeof(bool);
    std::memcpy(buffer + offset, (char*)&_node.keycount, sizeof(int));
    offset += sizeof(int);
    for (int i = 0; i < _node.keycount; i++)
    {
        std::memcpy(buffer + offset, (char*)&_node.keys[i], sizeof(int));
        offset += sizeof(int);
    }
    for (int i = 0; i < _node.keycount; i++)
    {
        unsigned long p;
        std::memcpy(buffer + offset, (char*)&_node.pointers[i], sizeof(unsigned long));
        offset += sizeof(unsigned long);
    }
    std::memcpy(buffer + offset, (char*)&_node.pre, sizeof(unsigned long));
    offset += sizeof(unsigned long);
    std::memcpy(buffer + offset, (char*)&_node.next, sizeof(unsigned long));
    //offset += sizeof(unsigned long);
    //std::memcpy(buffer + offset, (char*)&_node.parent, sizeof(unsigned long));
    treeNodeFile.write(buffer, PAGE_SIZE);
}

// 从文件读取一个节点并解析
void BpTree::deserialize(unsigned long point)
{
    treeNodeFile.seekg(point * PAGE_SIZE);
    char buffer[PAGE_SIZE] = "";
    treeNodeFile.read(buffer, PAGE_SIZE);
    //std::memcpy((char*)&node, buffer, sizeof node);
    int offset = 0;
    std::memcpy(&node.isLeaf, buffer + offset, sizeof(bool));
    offset += sizeof(bool);
    std::memcpy(&node.keycount, buffer + offset, sizeof(int));
    offset += sizeof(int);
    node.keys.clear();
    node.pointers.clear();
    for (int i = 0; i < node.keycount; i++)
    {
        int k;
        std::memcpy(&k, buffer + offset, sizeof(int));
        node.keys.push_back(k);
        offset += sizeof(int);
    }
    for (int i = 0; i < node.keycount; i++)
    {
        unsigned long p;
        std::memcpy(&p, buffer + offset, sizeof(unsigned long));
        node.pointers.push_back(p);
        offset += sizeof(unsigned long);
    }
    std::memcpy(&node.pre, buffer + offset, sizeof(unsigned long));
    offset += sizeof(unsigned long);
    std::memcpy(&node.next, buffer + offset, sizeof(unsigned long));
    offset += sizeof(unsigned long);
    //std::memcpy(&node.parent, buffer + offset, sizeof(unsigned long));
}

// 找到索引所在的叶节点的文件偏移
unsigned long BpTree::findLeaf(int k)
{
    if (root == -1)
        return -1;
    unsigned long p = root;
    deserialize(root);
    while (!node.isLeaf)
    {
        parentStackKey.push_back(p);
        parentStackNode.push_back(p);
        int it = std::lower_bound(node.keys.begin(), node.keys.end(), k) - node.keys.begin();
        if (it == node.keycount)
            it -= 1;
        p = node.pointers[it];
        deserialize(p);
    }
    return p;
}

// 插入叶节点
int BpTree::insertToLeaf(int k, unsigned long p)
{
    if (root == -1)
    {
        node.isLeaf = true;
        node.keycount = 1;
        node.keys.push_back(k);
        node.pointers.push_back(p);
        root = 0;
        head = 0;
        serialize(maxPoint++, node);
        return 1;
    }
    unsigned long leafP = findLeaf(k);
    int it = std::lower_bound(node.keys.begin(), node.keys.end(), k) - node.keys.begin();
    // 键值相同则插入失败
    if (it < node.keycount && k == node.keys[it])
    {
        parentStackNode.clear();
        parentStackKey.clear();
        return 0;
    }
    // 成功
    updateInnerNode(leafP, it, k);
    deserialize(leafP);
    node.keycount++;
    node.keys.insert(node.keys.begin() + it, k);
    node.pointers.insert(node.pointers.begin() + it, p);
    serialize(leafP, node);
    if (node.keycount > MAX_LEAF)
        splitNode(leafP);
    parentStackNode.clear();
    parentStackKey.clear();
    return 1;
}

// 更新内部节点的最大索引
void BpTree::updateInnerNode(unsigned long p, int it, int k)
{
    deserialize(p);
    //if (node.parent == -1)
    if (parentStackKey.empty())
        return;
    if (it >= node.keycount)
    {
        //unsigned long parent = node.parent;
        unsigned long parent = parentStackKey.back();
        parentStackKey.pop_back();
        deserialize(parent);
        it = std::lower_bound(node.keys.begin(), node.keys.end(), k) - node.keys.begin();
        node.keys[it - 1] = k;
        serialize(parent, node);
        updateInnerNode(parent, it, k);
    }
    parentStackKey.clear();
}

// 节点分裂
void BpTree::splitNode(unsigned long p)
{
    int mid = node.keycount / 2;
    int midKey = node.keys[mid - 1];
    int maxKey = node.keys.back();

    BpTreeNode newNode = BpTreeNode(node.isLeaf);
    unsigned long newPos = maxPoint++;
    for (int i = mid; i < node.keycount; i++)
    {
        newNode.keys.push_back(node.keys[i]);
        newNode.pointers.push_back(node.pointers[i]);
        newNode.keycount++;
        //if (!node.isLeaf)
        //{
        //    unsigned long tmp = node.pointers[i];
        //    deserialize(node.pointers[i]);
        //    node.parent = newPos;
        //    serialize(tmp, node);
        //    deserialize(p);
        //}
    }
    //deserialize(p);
    node.keys.resize(mid);
    node.pointers.resize(mid);
    node.keycount = mid;

    if (node.isLeaf)
    {
        unsigned long tmp = node.next;
        node.next = newPos;
        newNode.next = tmp;
        newNode.pre = p;
    }

    //if (node.parent == -1)
    if (parentStackNode.empty())
    {
        BpTreeNode newRoot = BpTreeNode(false);
        unsigned long newRootPos = maxPoint++;
        newRoot.keys.push_back(midKey);
        newRoot.keys.push_back(maxKey);
        newRoot.pointers.push_back(p);
        newRoot.pointers.push_back(newPos);
        newRoot.keycount = 2;
        //node.parent = newRootPos;
        //newNode.parent = newRootPos;
        root = newRootPos;
        serialize(p, node);
        serialize(newPos, newNode);
        serialize(newRootPos, newRoot);
    }
    else
    {
        serialize(p, node);
        serialize(newPos, newNode);
        insertToNode(parentStackNode.back(), p, newPos, midKey);
    }
}

// 插入非叶节点
void BpTree::insertToNode(unsigned long p, unsigned long leafp, unsigned long newLeafp, int k)
{
    parentStackNode.pop_back();
    //deserialize(newLeafp);
    //node.parent = p;
    //serialize(newLeafp, node);
    deserialize(p);
    int it = std::lower_bound(node.keys.begin(), node.keys.end(), k) - node.keys.begin();
    node.keys.insert(node.keys.begin() + it, k);
    node.pointers.insert(node.pointers.begin() + it, leafp);
    node.keycount++;
    node.pointers[it + 1] = newLeafp;
    serialize(p, node);
    parentStackKey = parentStackNode;
    updateInnerNode(p, it, k);
    deserialize(p);
    if (node.keycount > MAX_LEAF)
    {
        splitNode(p);
    }
}

// 删除叶节点
int BpTree::deleteLeaf(int k)
{
    if (root == -1)
        return 0;
    unsigned long leafP = findLeaf(k);
    int it = std::lower_bound(node.keys.begin(), node.keys.end(), k) - node.keys.begin();
    // 键值找不到则删除失败
    if (it >= node.keycount || k != node.keys[it])
        return 0;
    // 成功
    deleteNode(leafP, it, k);
    parentStackNode.clear();
    return 1;
}

void BpTree::deleteNode(unsigned long p, int it, int k)
{
    deserialize(p);
    // 如果可以直接删除
    if (node.keycount > MAX_LEAF / 2 || p == root)
    {
        node.pointers.erase(node.pointers.begin() + it);
        node.keys.erase(node.keys.begin() + it);
        node.keycount--;
        serialize(p, node);
        // 如果根节点只有一个内节点
        if (node.keycount == 0)
        {
            root = -1;
            head = -1;
            serialize(p, node);
            return;
        }
        parentStackKey = parentStackNode;
        updateInnerNode(p, node.keys.back());
        deserialize(p);
        // 如果根节点有两个内节点，且不是叶节点，则合并根节点
        if (node.keycount == 1 && !node.isLeaf)
        {
            deserialize(root);
            root = node.pointers[0];
        }
    }
    else
    {
        BpTreeNode parentNode;
        unsigned long parent;
        if (parentStackNode.empty())
            parent = -1;
        else
            parent = parentStackNode.back();
        parentStackNode.pop_back();
        deserialize(parent);
        parentNode = node;
        int pit = std::lower_bound(node.keys.begin(), node.keys.end(), k) - node.keys.begin();

        deserialize(p);
        unsigned long pre = node.pre;
        unsigned long next = node.next;
        if (!node.isLeaf)
        {
            if (pit > 0)
                pre = parentNode.pointers[pit - 1];
            if (pit < parentNode.keycount - 1)
                next = parentNode.pointers[pit + 1];
        }

        BpTreeNode preNode;
        if (pre != -1)
        {
            deserialize(pre);
            preNode = node;
        }
        BpTreeNode nextNode;
        if (next != -1)
        {
            deserialize(next);
            nextNode = node;
        }
        deserialize(p);

        // 向后合并
        if (pre == -1 || next != -1 && preNode.keycount >= nextNode.keycount)
        {
            node.keys.erase(node.keys.begin() + it);
            node.pointers.erase(node.pointers.begin() + it);
            node.keycount--;
            nextNode.keys.insert(nextNode.keys.begin(), node.keys.begin(), node.keys.end());
            nextNode.pointers.insert(nextNode.pointers.begin(), node.pointers.begin(), node.pointers.end());
            nextNode.keycount += node.keycount;
            // 叶节点则改变链接
            if (nextNode.isLeaf)
            {
                nextNode.pre = pre;
                if (pre != -1)
                    preNode.next = next;
                else
                    head = next;
            }
            serialize(p, node);
            if (pre != -1)
                serialize(pre, preNode);
            if (next != -1)
                serialize(next, nextNode);
            // 递归删除父节点的内部节点
            if (parent != -1)
                deleteNode(parent, pit, k);
            // 合并越界则分裂
            if (nextNode.keycount > MAX_LEAF)
            {
                deserialize(next);
                splitNode(next);
            }
        }
        // 向前合并
        else
        {
            node.keys.erase(node.keys.begin() + it);
            node.pointers.erase(node.pointers.begin() + it);
            node.keycount--;
            preNode.keys.insert(preNode.keys.end(), node.keys.begin(), node.keys.end());
            preNode.pointers.insert(preNode.pointers.end(), node.pointers.begin(), node.pointers.end());
            preNode.keycount += node.keycount;
            if (preNode.isLeaf)
            {
                preNode.next = next;
                if (next)
                    nextNode.pre = pre;
            }
            parentStackNode.push_back(parent);
            parentStackKey = parentStackNode;
            parentStackNode.pop_back();
            updateInnerNode(pre, preNode.keycount, preNode.keys.back());
            if (pre != -1)
                serialize(pre, preNode);
            if (next != -1)
                serialize(next, nextNode);
            if (parent != -1)
                deleteNode(parent, pit, k);
            if (nextNode.keycount > MAX_LEAF)
            {
                deserialize(pre);
                splitNode(pre);
            }
        }
    }

}

void BpTree::updateInnerNode(unsigned long p, int k)
{
    if (parentStackKey.empty())
        return;
    int it = std::lower_bound(node.keys.begin(), node.keys.end(), k) - node.keys.begin();
    if (it == node.keys.size() - 1)
    {
        unsigned long parent = parentStackKey.back();
        parentStackKey.pop_back();
        deserialize(parent);
        int it = std::lower_bound(node.keys.begin(), node.keys.end(), k) - node.keys.begin();
        node.keys[it] = k;
        serialize(parent, node);
        updateInnerNode(parent, k);
    }
    parentStackKey.clear();
}

//void BpTree::show()
//{
//    unsigned long p = head;
//    while (p != -1)
//    {
//        deserialize(p);
//        for (int i = 0; i < node.keycount; i++)
//        {
//            std::cout << node.keys[i] << " ";
//        }
//        std::cout << "| ";
//        p = node.next;
//    }
//    std::cout << std::endl;
//}

unsigned long BpTree::iter(Pair& pair)
{
    if (pair.k == -1)
        return -1;
    deserialize(pair.k);
    if (pair.v >= node.keycount)
    {
        pair.k = node.next;
        pair.v = 0;
    }
    if (pair.k == -1)
        return -1;
    else
    {
        deserialize(pair.k);
        return node.pointers[pair.v++];
    }
}

std::vector<unsigned long>& BpTree::iter(unsigned long& p)
{
    if (p == -1)
    {
        std::vector<unsigned long> empty;
        return empty;
    }
    deserialize(p);
    p = node.next;
    return node.pointers;
}

unsigned long BpTree::findPos(int k)
{
    unsigned long p = findLeaf(k);
    if (p == -1)
        return -1;
    parentStackKey.clear();
    parentStackNode.clear();
    for (int i = 0; i < node.keycount; i++)
    {
        if (k == node.keys[i])
        {
            return node.pointers[i];
        }
    }
    return -1;
}