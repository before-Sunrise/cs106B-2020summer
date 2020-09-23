// OurSet.h
// Header file for our Set of strings Class

#pragma once

#include <ostream>
#include "treenode.h"
using namespace std;

class OurSet {
public:
    // constructor
    OurSet();

    // destructor
    ~OurSet();

    // put value into set
    void add(string value);

    // remove value from set
    void remove(string value);

    // check if set contains the value
    bool contains(string value);

    // removes all elements from the set
    void clear();

    // return the number of elements in the array
    int size();

    // true if zero elements
    bool isEmpty();

    void printSetContents();

private:
    TreeNode* root;
    int numItems;           // how many elements are actually in our set

    void freeTree(TreeNode* tree);
    void addHelper(string value, TreeNode*& node);
    bool containsHelper(string value, TreeNode* node);
    void removeHelper(TreeNode*& node, string value);
    string removeLargestFrom(TreeNode*& root);
    void performDeletion(TreeNode*& toRemove);
    void inorderPrintTree(TreeNode* tree);

};
