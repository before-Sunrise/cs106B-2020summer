/*
 * CS106B Section Handout Test Harness: Section 7
 * ----------------------------------------------
 * These problems have been galvanized from years of
 * section handouts that have been worked on by numerous
 * instructors and TA's. Codified by Trip Master and Nick
 * Bowman for CS106B Spring 2020.
 *
 * A huge thank you to Keith Schwarz and Julie Zelenski
 * for creating an amazing testing harness!
 */
#include <iostream>
#include <string>
#include "testing/SimpleTest.h"
#include "testing/TextUtils.h"
#include "treenode.h"
#include "utility.h"
#include "error.h"
#include <cmath>
using namespace std;

/*
 * Balanced Trees (Code Writing)
 * --------------------------------------------------------
 * Write a function that takes in the root of a tree of integers
 * and returns whether or not the tree is balanced. A tree is
 * balanced if its left and right subtrees are balanced trees
 * whose heights differ by at most 1. The empty tree is defined
 * to be balanced. You solution may call on other functions
 * solved in this section handout.
 */

bool differInOne(int height1, int height2){
    if(abs(height1 - height2) <= 1){
        return true;
    }
    return false;
}

int height(TreeNode *node){
    if(node == nullptr){
        return 0;
    }
    return 1 + max(height(node->left), height(node->right));
}
bool isBalanced(TreeNode *node) {
    if(node == nullptr){
        return true;
    }
    return isBalanced(node->left) && isBalanced(node->right)
            && differInOne(height(node->left), height(node->right));

}

PROVIDED_TEST("Simple set of test cases for balanced function"){
    TreeNode *tree = nullptr;
    EXPECT(isBalanced(tree));

    tree = createTreeFromVector({/* Level 1*/ 1});
    EXPECT(isBalanced(tree));
    freeTree(tree);

    tree = createTreeFromVector({/* Level 1*/ 1, /* Level 2*/ 3, EMPTY});
    EXPECT(isBalanced(tree));
    freeTree(tree);

    tree = createTreeFromVector({/* Level 1*/ 1, /* Level 2*/ EMPTY, 3});
    EXPECT(isBalanced(tree));
    freeTree(tree);

    tree = createTreeFromVector({/* Level 1*/ 1, /* Level 2*/ 2, 3});
    EXPECT(isBalanced(tree));
    freeTree(tree);

    // handout examples

    tree = createTreeFromVector({/* Level 1*/ 0, /* Level 2*/ 0, 0, /* Level 3*/ EMPTY, EMPTY, 0, 0});
    EXPECT(isBalanced(tree));
    freeTree(tree);

    tree = createTreeFromVector({/* Level 1*/ 0, /* Level 2*/ 0, 0, /* Level 3*/ 0, EMPTY, EMPTY, EMPTY});
    EXPECT(isBalanced(tree));
    freeTree(tree);

    tree = createTreeFromVector({/* Level 1*/ 0, /* Level 2*/ 0, EMPTY, /* Level 3*/ 0, 0, EMPTY, EMPTY});
    EXPECT(!isBalanced(tree));
    freeTree(tree);

    tree = createTreeFromVector({/* Level 1*/ 0, /* Level 2*/ 0, 0, /* Level 3*/ 0, EMPTY, EMPTY, 0, /* Level 4*/ EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, 0, EMPTY});
    EXPECT(!isBalanced(tree));
    freeTree(tree);
}
