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
using namespace std;

bool isBSTRec(TreeNode* root, TreeNode* lowerBound, TreeNode* upperBound) {
    /* Base case: The empty tree is always valid.*/
    if (root == nullptr) return true;

    /* Otherwise, make sure this value is in the proper range. */
    if (lowerBound != nullptr && root->data <= lowerBound->data) return false;
    if (upperBound != nullptr && root->data >= upperBound->data) return false;

    /* Okay! We're in range. So now we just need to confirm that the left and
    * right subtrees are good as well. Notice how the range changes based on the
    * introduction of this node.
    */
    return isBSTRec(root->left, lowerBound, root)
           && isBSTRec(root->right, root, upperBound);
}

bool isBST(TreeNode* root) {
    return isBSTRec(root, nullptr, nullptr);
}

PROVIDED_TEST("Simple positive tests for isBST function"){
    TreeNode *tree = nullptr;
    EXPECT(isBST(tree));

    tree = createTreeFromVector({/* Level 1*/ 5});
    EXPECT(isBST(tree));

    freeTree(tree);
    tree = createTreeFromVector({/* Level 1*/ 5, /* Level 2*/ 3, EMPTY});
    EXPECT(isBST(tree));

    freeTree(tree);
    tree = createTreeFromVector({/* Level 1*/ 5, /* Level 2*/ EMPTY, 6});
    EXPECT(isBST(tree));

    freeTree(tree);
    tree = createTreeFromVector({/* Level 1*/ 5, /* Level 2*/ 3, 6});
    EXPECT(isBST(tree));

    freeTree(tree);
    tree = createTreeFromVector({/* Level 1*/ 5, /* Level 2*/ 3, 6, /* Level 3*/ 1, 4, EMPTY, EMPTY});
    EXPECT(isBST(tree));

    freeTree(tree);
    tree = createTreeFromVector({/* Level 1*/ 5, /* Level 2*/ 3, 6, /* Level 3*/ 1, 4, EMPTY, 9});
    EXPECT(isBST(tree));

    freeTree(tree);
    tree = createTreeFromVector({/* Level 1*/ 5, /* Level 2*/ 3, 6, /* Level 3*/ 1, 4, EMPTY, 9, /* Level 4*/ EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, 8, EMPTY});
    EXPECT(isBST(tree));

    freeTree(tree);
    tree = createTreeFromVector({/* Level 1*/ 5, /* Level 2*/ 3, 6, /* Level 3*/ 1, 4, EMPTY, 9, /* Level 4*/ EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, 8, 10});
    EXPECT(isBST(tree));

    freeTree(tree);
}

PROVIDED_TEST("Simple negative tests for isBST function"){
    TreeNode *tree = createTreeFromVector({5, 6});
    EXPECT(!isBST(tree));

    freeTree(tree);
    tree = createTreeFromVector({/* Level 1*/ 5, /* Level 2*/ 4, 3});
    EXPECT(!isBST(tree));

    freeTree(tree);
    tree = createTreeFromVector({/* Level 1*/ 5, /* Level 2*/ 4, 6, /* Level 3*/ 1, 7, EMPTY, EMPTY});
    EXPECT(!isBST(tree));

    freeTree(tree);
    tree = createTreeFromVector({/* Level 1*/ 5, /* Level 2*/ 4, 6, /* Level 3*/ 7, 1, EMPTY, EMPTY});
    EXPECT(!isBST(tree));

    freeTree(tree);
    tree = createTreeFromVector({/* Level 1*/ 5, /* Level 2*/ 4, 6, /* Level 3*/ 1, EMPTY, 8, 9});
    EXPECT(!isBST(tree));

    freeTree(tree);
    tree = createTreeFromVector({/* Level 1*/ 5, /* Level 2*/ 4, 6, /* Level 3*/ 1, EMPTY, EMPTY, 9, /* Level 4*/ EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, 1, 10});
    EXPECT(!isBST(tree));

    freeTree(tree);
}

