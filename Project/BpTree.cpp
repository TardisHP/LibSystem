#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "BpTree.h"

BpTreeNode::BpTreeNode(bool _isLeaf = false) : isLeaf(_isLeaf), keycount(0), pre(-1), next(-1), parent(-1) {}



BpTree::BpTree() : root(-1), head(-1), maxPoint(0)
{
    treeFile.open("Dataset/tree.dat", std::ios::in | std::ios::out | std::ios::binary | std::ios::ate);
    treeNodeFile.open("Dataset/treeNode.dat", std::ios::in | std::ios::out | std::ios::binary);
    if (0 != treeFile.tellg())
    {
        treeFile.seekg(0);
        char buffer[PAGE_SIZE] = "";
        treeFile.read(buffer, PAGE_SIZE);
        int offset = 0;
        std::memcpy((char*)&root, buffer + offset, sizeof(long));
        offset += sizeof(long);
        std::memcpy((char*)&head, buffer + offset, sizeof(long));
        offset += sizeof(long);
        std::memcpy((char*)&maxPoint, buffer + offset, sizeof(long));
    }
}

BpTree::~BpTree()
{
    treeFile.seekp(0);
    char buffer[PAGE_SIZE] = "";
    int offset = 0;
    std::memcpy(buffer + offset, (char*)&root, sizeof(long));
    offset += sizeof(long);
    std::memcpy(buffer + offset, (char*)&head, sizeof(long));
    offset += sizeof(long);
    std::memcpy(buffer + offset, (char*)&maxPoint, sizeof(long));
    treeFile.write(buffer, PAGE_SIZE);
    treeNodeFile.close();
    treeFile.close(); 
}

// 将node序列化加入到文件中
void BpTree::serialize(long point, BpTreeNode& _node)
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
        long p;
        std::memcpy(buffer + offset, (char*)&_node.pointers[i], sizeof(long));
        offset += sizeof(long);
    }
    std::memcpy(buffer + offset, (char*)&_node.pre, sizeof(long));
    offset += sizeof(long);
    std::memcpy(buffer + offset, (char*)&_node.next, sizeof(long));
    offset += sizeof(long);
    std::memcpy(buffer + offset, (char*)&_node.parent, sizeof(long));
    offset += sizeof(long);
    //std::memcpy(buffer, (char*)& _node, sizeof _node);
    treeNodeFile.write(buffer, PAGE_SIZE);
}

// 从文件读取一个节点并解析
void BpTree::deserialize(long point)
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
        long p;
        std::memcpy(&p, buffer + offset, sizeof(long));
        node.pointers.push_back(p);
        offset += sizeof(long);
    }
    std::memcpy(&node.pre, buffer + offset, sizeof(long));
    offset += sizeof(long);
    std::memcpy(&node.next, buffer + offset, sizeof(long));
    offset += sizeof(long);
    std::memcpy(&node.parent, buffer + offset, sizeof(long));
    offset += sizeof(long);
}

// 找到索引所在的叶节点的文件偏移
long BpTree::findLeaf(int k)
{
    long p = root;
    deserialize(root);
    while (!node.isLeaf)
    {
        int it = std::lower_bound(node.keys.begin(), node.keys.end(), k) - node.keys.begin();
        if (it == node.keys.size())
            it -= 1;
        p = node.pointers[it];
        deserialize(p);
    }
    return p;
}

// 插入叶节点
void BpTree::insertToLeaf(int k, long p)
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
        return;
    }
    long leafP = findLeaf(k);
    int it = std::lower_bound(node.keys.begin(), node.keys.end(), k) - node.keys.begin();
    // 键值相同则插入失败
    if (it < node.keys.size() && k == node.keys[it])
    {
        std::cout << k << " insert FAILED!" << std::endl;
        return;
    }
    // 成功
    updateInnerNode(leafP, it, k);
    deserialize(leafP);
    node.keycount++;
    node.keys.insert(node.keys.begin() + it, k);
    node.pointers.insert(node.pointers.begin() + it, p);
    serialize(leafP, node);
    if (node.keys.size() > MAX_LEAF)
        splitNode(leafP);
}

// 更新内部节点的最大索引
void BpTree::updateInnerNode(long p, int it, int k)
{
    //deserialize(p);
    if (node.parent == -1)
        return;
    if (it >= node.keys.size())
    {
        long parent = node.parent;
        deserialize(parent);
        it = std::lower_bound(node.keys.begin(), node.keys.end(), k) - node.keys.begin();
        node.keys[it - 1] = k;
        serialize(parent, node);
        updateInnerNode(parent, it, k);
    }
}

// 节点分裂
void BpTree::splitNode(long p)
{
    int mid = node.keys.size() / 2;
    int midKey = node.keys[mid - 1];
    int maxKey = node.keys.back();

    BpTreeNode newNode = BpTreeNode(node.isLeaf);
    long newPos = maxPoint++;
    for (int i = mid; i < node.keys.size(); i++)
    {
        newNode.keys.push_back(node.keys[i]);
        newNode.pointers.push_back(node.pointers[i]);
        newNode.keycount++;
        if (!node.isLeaf)
        {
            long tmp = node.pointers[i];
            deserialize(node.pointers[i]);
            node.parent = newPos;
            serialize(tmp, node);
            deserialize(p);
        }
    }
    //deserialize(p);
    node.keys.resize(mid);
    node.pointers.resize(mid);
    node.keycount = mid;

    if (node.isLeaf)
    {
        long tmp = node.next;
        node.next = newPos;
        newNode.next = tmp;
        newNode.pre = p;
    }

    if (node.parent == -1)
    {
        BpTreeNode newRoot = BpTreeNode(false);
        long newRootPos = maxPoint++;
        newRoot.keys.push_back(midKey);
        newRoot.keys.push_back(maxKey);
        newRoot.pointers.push_back(p);
        newRoot.pointers.push_back(newPos);
        newRoot.keycount = 2;
        node.parent = newRootPos;
        newNode.parent = newRootPos;
        root = newRootPos;
        serialize(p, node);
        serialize(newPos, newNode);
        serialize(newRootPos, newRoot);
    }
    else
    {
        serialize(p, node);
        serialize(newPos, newNode);
        insertToNode(node.parent, p, newPos, midKey);
    }
}

// 插入非叶节点
void BpTree::insertToNode(long p, long leafp, long newLeafp, int k)
{
    deserialize(newLeafp);
    node.parent = p;
    serialize(newLeafp, node);
    deserialize(p);
    int it = std::lower_bound(node.keys.begin(), node.keys.end(), k) - node.keys.begin();
    node.keys.insert(node.keys.begin() + it, k);
    node.pointers.insert(node.pointers.begin() + it, leafp);
    node.keycount++;
    node.pointers[it + 1] = newLeafp;
    serialize(p, node);
    updateInnerNode(p, it, k);
    deserialize(p);
    if (node.keys.size() > MAX_LEAF)
    {
        splitNode(p);
    }
}

//// 删除叶节点
//void BpTree::deleteLeaf(int k)
//{
//    if (!root)
//    {
//        std::cout << k << "EMPTY!" << std::endl;
//        return;
//    }
//    BpTreeNode* leaf = findLeaf(k);
//    int it = std::lower_bound(leaf->keys.begin(), leaf->keys.end(), k) - leaf->keys.begin();
//    // 键值找不到则删除失败
//    if (it >= leaf->keys.size() || k != leaf->keys[it])
//    {
//        std::cout << k << " delete FAILED!" << std::endl;
//        return;
//    }
//    // 成功
//    deleteNode(leaf, it, k);
//}
//
//void BpTree::deleteNode(BpTreeNode* node, int it, int k)
//{
//    // 如果可以直接删除
//    if (node->keys.size() > order / 2 || node == root)
//    {
//        node->children.erase(node->children.begin() + it);
//        node->keys.erase(node->keys.begin() + it);
//        if (node->isLeaf)
//            node->values.erase(node->values.begin() + it);
//        // 如果根节点只有一个内节点
//        if (node->keys.size() == 0)
//        {
//            delete root;
//            root = nullptr;
//            head = nullptr;
//            return;
//        }
//        updateInnerNode(node, node->keys.back());
//        // 如果根节点有两个内节点，且不是叶节点，则合并根节点
//        if (node->keys.size() == 1 && !node->isLeaf)
//        {
//            BpTreeNode* tmp = root;
//            root = root->children[0];
//            root->parent = nullptr;
//            delete tmp;
//        }
//    }
//    else
//    {
//        BpTreeNode* parent = node->parent;
//        int pit = std::lower_bound(parent->keys.begin(), parent->keys.end(), k) - parent->keys.begin();
//        BpTreeNode* pre = node->pre;
//        BpTreeNode* next = node->next;
//        if (!node->isLeaf)
//        {
//            if (pit > 0)
//                pre = parent->children[pit - 1];
//            if (pit < parent->keys.size() - 1)
//                next = parent->children[pit + 1];
//        }
//        // 向后合并
//        if (!pre || next && pre->keys.size() >= next->keys.size())
//        {
//            node->keys.erase(node->keys.begin() + it);
//            next->keys.insert(next->keys.begin(), node->keys.begin(), node->keys.end());
//            node->children.erase(node->children.begin() + it);
//            next->children.insert(next->children.begin(), node->children.begin(), node->children.end());
//            if (node->isLeaf)
//            {
//                node->values.erase(node->values.begin() + it);
//                next->values.insert(next->values.begin(), node->values.begin(), node->values.end());
//                delete node;
//            }
//            else
//            {
//                // 改变合并的children的parent
//                for (int i = 0; i < node->keys.size(); i++)
//                {
//                    next->children[i]->parent = next;
//                }
//                node->children.clear();
//                delete node;
//            }
//            // 递归删除父节点的内部节点
//            if (parent)
//                deleteNode(parent, pit, k);
//            // 叶节点则改变链接
//            if (next->isLeaf)
//            {
//                next->pre = pre;
//                if (pre)
//                    pre->next = next;
//                else
//                    head = next;
//            }
//            // 合并越界则分裂
//            if (next->keys.size() > order)
//                splitNode(next);
//        }
//        // 向前合并
//        else
//        {
//            node->keys.erase(node->keys.begin() + it);
//            pre->keys.insert(pre->keys.end(), node->keys.begin(), node->keys.end());
//            node->children.erase(node->children.begin() + it);
//            pre->children.insert(pre->children.end(), node->children.begin(), node->children.end());
//            if (node->isLeaf)
//            {
//                node->values.erase(node->values.begin() + it);
//                pre->values.insert(pre->values.end(), node->values.begin(), node->values.end());
//                delete node;
//            }
//            else
//            {
//                for (int i = pre->keys.size() - node->keys.size(); i < pre->keys.size(); i++)
//                {
//                    pre->children[i]->parent = pre;
//                }
//                node->children.clear();
//                delete node;
//            }
//            updateInnerNode(pre, pre->keys.size(), pre->keys.back());
//            if (parent)
//                deleteNode(parent, pit, k);
//            if (pre->isLeaf)
//            {
//                pre->next = next;
//                if (next)
//                    next->pre = pre;
//            }
//            if (pre->keys.size() > order)
//                splitNode(pre);
//        }
//    }
//
//}
//
//BpTreeNode* BpTree::getHead()
//{
//    return head;
//}

void BpTree::show()
{
    long p = head;
    while (p != -1)
    {
        deserialize(p);
        for (int i = 0; i < node.keys.size(); i++)
        {
            std::cout << node.keys[i] << " ";
        }
        std::cout << "| ";
        p = node.next;
    }
    std::cout << std::endl;
}

//
//void BpTree::updateInnerNode(BpTreeNode* node, int k)
//{
//    if (!node->parent)
//        return;
//    int it = std::lower_bound(node->keys.begin(), node->keys.end(), k) - node->keys.begin();
//    if (it == node->keys.size() - 1)
//    {
//        BpTreeNode* parent = node->parent;
//        int it = std::lower_bound(parent->keys.begin(), parent->keys.end(), k) - parent->keys.begin();
//        parent->keys[it] = k;
//        updateInnerNode(parent, k);
//    }
//}
//

//

//
long BpTree::findPos(int k)
{
    long p = findLeaf(k);
    for (int i = 0; i < node.keys.size(); i++)
    {
        if (k == node.keys[i])
        {
            return node.pointers[i];
        }
    }
    return -1;
}