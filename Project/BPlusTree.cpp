#include <iostream>
#include <fstream>
#include <string>
#include <vector>

class BpTreeNode
{
public:
    bool isLeaf;
    std::vector<int> keys;
    std::vector<BpTreeNode*> children;
    std::vector<int> values;
    BpTreeNode* next;
    BpTreeNode* parent;

    BpTreeNode(bool _isLeaf = false) : isLeaf(_isLeaf), next(nullptr), parent(nullptr) {};
};

class BTree
{
public:
    BTree() : root(nullptr), head(nullptr), order(4) {};
    BpTreeNode* findLeaf(int k);
    void insertToLeaf(int k, int v);
    void display();
private:
    BpTreeNode* root;
    BpTreeNode* head;
    int order;
    void splitLeafNode(BpTreeNode* leaf);
    void insertToNode(BpTreeNode* node, BpTreeNode* leaf, BpTreeNode* newLeaf, int k);
    void splitNode(BpTreeNode* node);
    void updateIndex(BpTreeNode* node, int it, int k);
};

BpTreeNode* BTree::findLeaf(int k)
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

void BTree::updateIndex(BpTreeNode* node, int it, int k)
{
    if (!node->parent)
        return;
    if (it >= node->keys.size())
    {
        BpTreeNode* parent = node->parent;
        it = std::lower_bound(parent->keys.begin(), parent->keys.end(), k) - parent->keys.begin();
        parent->keys[it - 1] = k;
        updateIndex(parent, it, k);
    }
}

void BTree::insertToLeaf(int k, int v)
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
    updateIndex(leaf, it, k);
    /*if (it >= leaf->keys.size() && leaf->parent)
    {
        leaf->parent->keys.back() = k;
    }*/
    leaf->keys.insert(leaf->keys.begin() + it, k);
    leaf->children.insert(leaf->children.begin() + it, nullptr);
    leaf->values.insert(leaf->values.begin() + it, v);
    if (leaf->keys.size() > order)
    {
        splitLeafNode(leaf);
    }
}

void BTree::splitLeafNode(BpTreeNode* leaf)
{
    int mid = leaf->keys.size() / 2;
    int midKey = leaf->keys[mid - 1];
    int maxKey = leaf->keys.back();

    BpTreeNode* newLeaf = new BpTreeNode(true);
    for (int i = mid; i < leaf->keys.size(); i++)
    {
        newLeaf->keys.push_back(leaf->keys[i]);
        newLeaf->children.push_back(nullptr);
        newLeaf->values.push_back(leaf->values[i]);
    }
    leaf->keys.resize(mid);
    leaf->children.resize(mid);
    leaf->values.resize(mid);

    BpTreeNode* tmp = leaf->next;
    leaf->next = newLeaf;
    newLeaf->next = tmp;

    if (!leaf->parent)
    {
        root = new BpTreeNode(false);
        root->keys.push_back(midKey);
        root->keys.push_back(maxKey);
        root->children.push_back(leaf);
        root->children.push_back(newLeaf);
        leaf->parent = root;
        newLeaf->parent = root;
    }
    else
        insertToNode(leaf->parent, leaf, newLeaf, midKey);
}

void BTree::insertToNode(BpTreeNode* node, BpTreeNode* leaf, BpTreeNode* newLeaf, int k)
{
    newLeaf->parent = node;
    int it = std::lower_bound(node->keys.begin(), node->keys.end(), k) - node->keys.begin();
    node->keys.insert(node->keys.begin() + it, k);
    node->children.insert(node->children.begin() + it, leaf);
    node->children[it + 1] = newLeaf;
    updateIndex(node, it, k);
    /*if (it >= node->keys.size() && node->parent)
    {
        node->parent->keys.back() = k;
    }*/
    if (node->keys.size() > order)
    {
        splitNode(node);
    }
}

void BTree::splitNode(BpTreeNode* node)
{
    int mid = node->keys.size() / 2;
    int midKey = node->keys[mid - 1];
    int maxKey = node->keys.back();

    BpTreeNode* newNode = new BpTreeNode(false);
    for (int i = mid; i < node->keys.size(); i++)
    {
        newNode->keys.push_back(node->keys[i]);
        newNode->children.push_back(node->children[i]);
        node->children[i]->parent = newNode;
    }
    node->keys.resize(mid);
    node->children.resize(mid);

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

void BTree::display()
{
    BpTreeNode* p = head;
    while (p)
    {
        for (auto v : p->values)
            std::cout << v << " ";
        std::cout << " | ";
        p = p->next;
    }

}

int main()
{
    BTree tree;
    for (int i = 1; i <= 20; i+=2)
    {
        tree.insertToLeaf(i, i);
        tree.display();
        std::cout << std::endl;
    }
    tree.insertToLeaf(4, 4);
    tree.insertToLeaf(6, 6);
    tree.insertToLeaf(8, 8);
    tree.insertToLeaf(12, 12);
    tree.display();

}