#pragma once

#include "treenode.h"
#include "vector.h"

#define EMPTY INT_MAX

TreeNode* createTreeFromVector(Vector<int> nums);
void freeTree(TreeNode* root);
bool treeEqual(TreeNode* a, TreeNode *b);
void printTree(TreeNode *root);

