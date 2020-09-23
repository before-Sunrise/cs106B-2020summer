/*
 * This file contains supporting code that we used to help
 * us with working with trees.
 */

#include "treenode.h"
#pragma once

// function prototype declarations
void prettyPrint(TreeNode* tree);
TreeNode* buildExampleTree();
void preorderPrintTree(TreeNode* tree);
void inorderPrintTree(TreeNode* tree);
void postorderPrintTree(TreeNode* tree);
void freeTree(TreeNode* tree);
