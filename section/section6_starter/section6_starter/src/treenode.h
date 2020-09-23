#pragma once

#include "testing/MemoryDiagnostics.h"

struct TreeNode {
    int data;
    TreeNode *left;
    TreeNode *right;

    // default constructor does not initialize
    TreeNode() {}   

    // 3-arg constructor sets fields from arguments
    TreeNode(int d, TreeNode* l, TreeNode* r) {  
        data = d;
        left = l;
        right = r;
    }

    TRACK_ALLOCATIONS_OF(TreeNode);
};
