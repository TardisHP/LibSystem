#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "BpTree.h"

BpTreeNode::BpTreeNode(bool _isLeaf = false) : isLeaf(_isLeaf), next(nullptr), parent(nullptr) {};


BpTree::BpTree() : root(nullptr), head(nullptr), order(4) {};

// 找到索引所在的叶节点
BpTreeNode* BpTree::findLeaf(int k)
{
    BpTreeNode* p = root;
    while (!p->isLeaf)
    {
        int it = std::lower_bound(p->keys.begin(), p->keys.end(), k) - p->keys.begin();
        if (it == p->keys.size())
            it -= 1;
        p = p->children[it];
    }
    return p;
}

// 插入叶节点
void BpTree::insertToLeaf(int k, std::streampos v)
{
    if (!root)
    {
        BpTreeNode* node = new BpTreeNode(true);
        node->keys.push_back(k);
        node->children.push_back(nullptr);
        node->values.push_back(v);
        root = node;
        head = node;
        return;
    }
    BpTreeNode* leaf = findLeaf(k);
    int it = std::lower_bound(leaf->keys.begin(), leaf->keys.end(), k) - leaf->keys.begin();
    // 键值相同则插入失败
    if (it < leaf->keys.size() && k == leaf->keys[it])
    {
        std::cout << k << " insert FAILED!" << std::endl;
        return;
    }
    // 成功
    updateInnerNode(leaf, it, k);
    leaf->keys.insert(leaf->keys.begin() + it, k);
    leaf->children.insert(leaf->children.begin() + it, nullptr);
    leaf->values.insert(leaf->values.begin() + it, v);
    if (leaf->keys.size() > order)
    {
        splitNode(leaf);
    }
}

BpTreeNode* BpTree::getHead()
{
    return head;
}

// 更新内部节点的最大索引
void BpTree::updateInnerNode(BpTreeNode* node, int it, int k)
{
    if (!node->parent)
        return;
    if (it >= node->keys.size())
    {
        BpTreeNode* parent = node->parent;
        it = std::lower_bound(parent->keys.begin(), parent->keys.end(), k) - parent->keys.begin();
        parent->keys[it - 1] = k;
        updateInnerNode(parent, it, k);
    }
}

// 插入非叶节点
void BpTree::insertToNode(BpTreeNode* node, BpTreeNode* leaf, BpTreeNode* newLeaf, int k)
{
    newLeaf->parent = node;
    int it = std::lower_bound(node->keys.begin(), node->keys.end(), k) - node->keys.begin();
    node->keys.insert(node->keys.begin() + it, k);
    node->children.insert(node->children.begin() + it, leaf);
    node->children[it + 1] = newLeaf;
    updateInnerNode(node, it, k);
    if (node->keys.size() > order)
    {
        splitNode(node);
    }
}

// 节点分裂
void BpTree::splitNode(BpTreeNode* node)
{
    int mid = node->keys.size() / 2;
    int midKey = node->keys[mid - 1];
    int maxKey = node->keys.back();

    BpTreeNode* newNode = new BpTreeNode(node->isLeaf);
    for (int i = mid; i < node->keys.size(); i++)
    {
        newNode->keys.push_back(node->keys[i]);
        newNode->children.push_back(node->children[i]);
        if (node->isLeaf)
            newNode->values.push_back(node->values[i]);
        else
            node->children[i]->parent = newNode;
    }
    node->keys.resize(mid);
    node->children.resize(mid);
    if (node->isLeaf)
    {
        node->values.resize(mid);
        BpTreeNode* tmp = node->next;
        node->next = newNode;
        newNode->next = tmp;
    }

    if (!node->parent)
    {
        root = new BpTreeNode(false);
        root->keys.push_back(midKey);
        root->keys.push_back(maxKey);
        root->children.push_back(node);
        root->children.push_back(newNode);
        node->parent = root;
        newNode->parent = root;
    }
    else
        insertToNode(node->parent, node, newNode, midKey);
}

std::streampos BpTree::findPos(int k)
{
    BpTreeNode* p = findLeaf(k);
    for (int i = 0; i < p->keys.size(); i++)
    {
        if (k == p->keys[i])
        {
            return p->values[i];
        }
    }
    return -1;
}