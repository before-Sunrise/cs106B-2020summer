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
#include "testing/TestDriver.h"
#include "treenode.h"
#include "utility.h"
#include "error.h"
#include "set.h"
using namespace std;

/*
 * Counting Left Nodes (Code Writing)
 * --------------------------------------------------------
 * Write a function that takes in the root of a tree of
 * integers and returns the number of left children in the tree.
 * A left child is a node that appears as the root of a
 * left-hand subtree of another node.
 */

//
//flag: 1 represents node is a left subtree, 0 right subtree/ root node
int countLeftNodesHelp(TreeNode *node, int flag){
    if(node == nullptr){
        return 0;
    }
    return flag + countLeftNodesHelp(node->left, 1) + countLeftNodesHelp(node->right, 0);
}

int countLeftNodes(TreeNode *node) {
    return countLeftNodesHelp(node, 0);

}


/* official solution
int countLeftNodes(TreeNode *node) {
    if (node == nullptr) {
        return 0;
    } else if (node->left == nullptr) {
        return countLeftNodes(node->right);
    } else {
        return 1 + countLeftNodes(node->left) + countLeftNodes(node->right);
    }
}
*/
PROVIDED_TEST("Simple set of test cases for countLeft function"){
    TreeNode *tree = nullptr;
    EXPECT_EQUAL(0, countLeftNodes(tree));

    tree = createTreeFromVector({/* Level 1*/ 1});
    EXPECT_EQUAL(0, countLeftNodes(tree));
    freeTree(tree);

    tree = createTreeFromVector({/* Level 1*/ 1, /* Level 2*/ 3, EMPTY});
    EXPECT_EQUAL(1, countLeftNodes(tree));
    freeTree(tree);

    tree = createTreeFromVector({/* Level 1*/ 1, /* Level 2*/ EMPTY, 3});
    EXPECT_EQUAL(0, countLeftNodes(tree));
    freeTree(tree);

    tree = createTreeFromVector({/* Level 1*/ 1, /* Level 2*/ 2, 3});
    EXPECT_EQUAL(1, countLeftNodes(tree));
    freeTree(tree);

    tree = createTreeFromVector({/* Level 1*/ 1, /* Level 2*/ EMPTY, 3, /* Level 3*/ EMPTY, EMPTY, EMPTY, 2});
    EXPECT_EQUAL(0, countLeftNodes(tree));
    freeTree(tree);

    tree = createTreeFromVector({/* Level 1*/ 1, /* Level 2*/ 2, 3, /* Level 3*/ 4, EMPTY, EMPTY, 2});
    EXPECT_EQUAL(2, countLeftNodes(tree));
    freeTree(tree);

    tree = createTreeFromVector({/* Level 1*/ 1, /* Level 2*/ 2, 3, /* Level 3*/ 4, EMPTY, EMPTY, 2, /* Level 4 */ 5});
    EXPECT_EQUAL(3, countLeftNodes(tree));
    freeTree(tree);

    // example from handout 1(a)
    tree = createTreeFromVector({/* Level 1 */ 3, /* Level 2 */ 5, 2, /* Level 3 */ 1, EMPTY, 4, 6});
    EXPECT_EQUAL(3, countLeftNodes(tree));
    freeTree(tree);
}
