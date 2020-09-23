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
#include "testing/SimpleTest.h"
#include "testing/TextUtils.h"
#include "treenode.h"
#include "utility.h"
#include "error.h"
#include "stack.h"
#include "set.h"
using namespace std;

/*
 * Pruning Tree Leaves (Code Writing)
 * --------------------------------------------------------
 * Write a function that accepts a reference to a pointer
 * to a TreeNode and removes the leaf nodes from a tree. A
 * leaf is a node that has empty left and right subtrees.
 * If your function is called on an empty tree, it does not
 * change the tree because there are no nodes of any kind
 * (leaf or not). You must free the memory for any removed
 * nodes.
 */

void removeLeaves(TreeNode*& node) {
    if(node == nullptr){
        return;
    }else if(node->left == nullptr && node->right == nullptr){
        delete node;
        node = nullptr; //因为传引用，所以这里对node的赋值可以影响到上层调用时传入的参数
    }else{ //
        removeLeaves(node->left);
        removeLeaves(node->right);
    }



}

PROVIDED_TEST("Simple set of test cases for countLeft function"){
    TreeNode *tree = nullptr;
    TreeNode *soln = nullptr;
    removeLeaves(tree);
    EXPECT(treeEqual(soln, tree));

    // example from handout

    tree = createTreeFromVector({/* Level 1 */ 7, /* Level 2 */ 3, 9, /* Level 3 */ 1, 4, 6, 8, /* Level 4 */ EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, 0});
    soln = createTreeFromVector({/* Level 1 */ 7, /* Level 2 */ 3, 9, /* Level 3 */ EMPTY, EMPTY, EMPTY, 8});

    removeLeaves(tree);
    EXPECT(treeEqual(soln, tree));

    freeTree(soln);
    soln = createTreeFromVector({/* Level 1 */ 7,  /* Level 2 */ EMPTY, 9});

    removeLeaves(tree);
    EXPECT(treeEqual(soln, tree));

    freeTree(soln);
    soln = createTreeFromVector({/* Level 1 */ 7});

    removeLeaves(tree);
    EXPECT(treeEqual(soln, tree));

    freeTree(soln);
    soln = nullptr;

    removeLeaves(tree);
    EXPECT(treeEqual(soln, tree));
}
