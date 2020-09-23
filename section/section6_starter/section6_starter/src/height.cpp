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
#include "vector.h"
using namespace std;

/*
 * Tree Height (Code Writing)
 * --------------------------------------------------------
 * Write a function that calculates the height of the provided
 * tree. The height of a tree is defined to be the number of
 * levels that a tree has, or phrased differently, the number
 * of nodes along the longest path from the root to a leaf.
 * For example, an empty tree has height 0. A tree of one node
 * has height 1. A node with one or two leaves as children has
 * height 2, etc.
 */


int height(TreeNode *node) {
    if(node == nullptr){
        return 0;
    }
    return max(height(node->left), height(node->right)) + 1;
}


PROVIDED_TEST("Simple set of test cases for height function"){
    TreeNode *tree = nullptr;
    EXPECT_EQUAL(0, height(tree));

    tree = createTreeFromVector({/* Level 1*/ 1});
    EXPECT_EQUAL(1, height(tree));
    freeTree(tree);

    tree = createTreeFromVector({/* Level 1*/ 1, /* Level 2*/ 3, EMPTY});
    EXPECT_EQUAL(2, height(tree));
    freeTree(tree);

    tree = createTreeFromVector({/* Level 1*/ 1, /* Level 2*/ EMPTY, 3});
    EXPECT_EQUAL(2, height(tree));
    freeTree(tree);

    tree = createTreeFromVector({/* Level 1*/ 1, /* Level 2*/ 2, 3});
    EXPECT_EQUAL(2, height(tree));
    freeTree(tree);

    tree = createTreeFromVector({/* Level 1*/ 1, /* Level 2*/ EMPTY, 3, /* Level 3*/ EMPTY, EMPTY, EMPTY, 2});
    EXPECT_EQUAL(3, height(tree));
    freeTree(tree);

    tree = createTreeFromVector({/* Level 1*/ 1, /* Level 2*/ 2, 3, /* Level 3*/ 4, EMPTY, EMPTY, 2, /* Level 4 */ 5});
    EXPECT_EQUAL(4, height(tree));
    freeTree(tree);
}


