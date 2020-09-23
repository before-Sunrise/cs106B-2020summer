/*
 * This file contains supporting code that we used to help
 * us with working with trees.
 */

#pragma once

struct TreeNode {
    std::string data;
    TreeNode* left;
    TreeNode* right;

    // default constructor does not initialize
    TreeNode() {}

    // 3-arg constructor sets fields from arguments
    TreeNode(std::string d, TreeNode* l, TreeNode* r) {
        data = d;
        left = l;
        right = r;
    }
};

// function prototype declarations
void prettyPrint(TreeNode* tree);
TreeNode* buildExampleTree();
void preorderPrintTree(TreeNode* tree);
void inorderPrintTree(TreeNode* tree);
void postorderPrintTree(TreeNode* tree);
