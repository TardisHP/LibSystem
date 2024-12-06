#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "BpTree.h"

BpTreeNode::BpTreeNode(bool _isLeaf = false) : isLeaf(_isLeaf), pre(nullptr), next(nullptr), parent(nullptr) {};



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

// 删除叶节点
void BpTree::deleteLeaf(int k)
{
    if (!root)
    {
        std::cout << k << "EMPTY!" << std::endl;
        return;
    }
    BpTreeNode* leaf = findLeaf(k);
    int it = std::lower_bound(leaf->keys.begin(), leaf->keys.end(), k) - leaf->keys.begin();
    // 键值找不到则删除失败
    if (it >= leaf->keys.size() || k != leaf->keys[it])
    {
        std::cout << k << " delete FAILED!" << std::endl;
        return;
    }
    // 成功
    deleteNode(leaf, it, k);
}

void BpTree::deleteNode(BpTreeNode* node, int it, int k)
{
    // 如果可以直接删除
    if (node->keys.size() > order / 2 || node == root)
    {
        node->children.erase(node->children.begin() + it);
        node->keys.erase(node->keys.begin() + it);
        if (node->isLeaf)
            node->values.erase(node->values.begin() + it);
        // 如果根节点只有一个内节点
        if (node->keys.size() == 0)
        {
            delete root;
            root = nullptr;
            head = nullptr;
            return;
        }
        updateInnerNode(node, node->keys.back());
        // 如果根节点有两个内节点，且不是叶节点，则合并根节点
        if (node->keys.size() == 1 && !node->isLeaf)
        {
            BpTreeNode* tmp = root;
            root = root->children[0];
            root->parent = nullptr;
            delete tmp;
        }
    }
    else
    {
        BpTreeNode* parent = node->parent;
        int pit = std::lower_bound(parent->keys.begin(), parent->keys.end(), k) - parent->keys.begin();
        BpTreeNode* pre = node->pre;
        BpTreeNode* next = node->next;
        if (!node->isLeaf)
        {
            if (pit > 0)
                pre = parent->children[pit - 1];
            if (pit < parent->keys.size() - 1)
                next = parent->children[pit + 1];
        }
        // 向后合并
        if (!pre || next && pre->keys.size() >= next->keys.size())
        {
            node->keys.erase(node->keys.begin() + it);
            next->keys.insert(next->keys.begin(), node->keys.begin(), node->keys.end());
            node->children.erase(node->children.begin() + it);
            next->children.insert(next->children.begin(), node->children.begin(), node->children.end());
            if (node->isLeaf)
            {
                node->values.erase(node->values.begin() + it);
                next->values.insert(next->values.begin(), node->values.begin(), node->values.end());
                delete node;
            }
            else
            {
                // 改变合并的children的parent
                for (int i = 0; i < node->keys.size(); i++)
                {
                    next->children[i]->parent = next;
                }
                node->children.clear();
                delete node;
            }
            // 递归删除父节点的内部节点
            if (parent)
                deleteNode(parent, pit, k);
            // 叶节点则改变链接
            if (next->isLeaf)
            {
                next->pre = pre;
                if (pre)
                    pre->next = next;
                else
                    head = next;
            }
            // 合并越界则分裂
            if (next->keys.size() > order)
                splitNode(next);
        }
        // 向前合并
        else
        {
            node->keys.erase(node->keys.begin() + it);
            pre->keys.insert(pre->keys.end(), node->keys.begin(), node->keys.end());
            node->children.erase(node->children.begin() + it);
            pre->children.insert(pre->children.end(), node->children.begin(), node->children.end());
            if (node->isLeaf)
            {
                node->values.erase(node->values.begin() + it);
                pre->values.insert(pre->values.end(), node->values.begin(), node->values.end());
                delete node;
            }
            else
            {
                for (int i = pre->keys.size() - node->keys.size(); i < pre->keys.size(); i++)
                {
                    pre->children[i]->parent = pre;
                }
                node->children.clear();
                delete node;
            }
            updateInnerNode(pre, pre->keys.size(), pre->keys.back());
            if (parent)
                deleteNode(parent, pit, k);
            if (pre->isLeaf)
            {
                pre->next = next;
                if (next)
                    next->pre = pre;
            }
            if (pre->keys.size() > order)
                splitNode(pre);
        }
    }

}

BpTreeNode* BpTree::getHead()
{
    return head;
}

void BpTree::show()
{
    BpTreeNode* p = head;
    while (p)
    {
        for (int i = 0; i < p->keys.size(); i++)
        {
            std::cout << p->keys[i] << " ";
        }
        std::cout << "| ";
        p = p->next;
    }
    std::cout << std::endl;
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

void BpTree::updateInnerNode(BpTreeNode* node, int k)
{
    if (!node->parent)
        return;
    int it = std::lower_bound(node->keys.begin(), node->keys.end(), k) - node->keys.begin();
    if (it == node->keys.size() - 1)
    {
        BpTreeNode* parent = node->parent;
        int it = std::lower_bound(parent->keys.begin(), parent->keys.end(), k) - parent->keys.begin();
        parent->keys[it] = k;
        updateInnerNode(parent, k);
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
        newNode->pre = node;
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