#include "utility.h"

using namespace std;

void createTreeRec(TreeNode*& node, Vector<int> nums, int index) {
    if (index >= nums.size()) return;
    if (nums[index] != EMPTY){
        node = new TreeNode(nums[index], nullptr, nullptr);
        createTreeRec(node->left, nums, 2*index + 1);
        createTreeRec(node->right, nums, 2*index + 2);
    }
}

/* This function takes in a vector representing the level-order
 * representation of a tree and turns it into a tree, returning
 * the root of the newly created tree. Empty locations in the
 * level order traversal are represented with the sentinel value
 * EMPTY.
 */
TreeNode* createTreeFromVector(Vector<int> nums){
    TreeNode *root = nullptr;
    createTreeRec(root, nums, 0);
    return root;
}

void freeTree(TreeNode* root){
	if (root == nullptr) return;
	freeTree(root->left);
    freeTree(root->right);
	delete root;
}

bool treeEqual(TreeNode *a, TreeNode *b){
    if (a == nullptr || b == nullptr) return a == b;

    return a->data == b->data
           && treeEqual(a->left, b->left)
           && treeEqual(a->right, b->right);
}

void printTree(TreeNode* root){
    if (root == nullptr) return;
    cout << root->data << " ";
    printTree(root->left);
    printTree(root->right);
}
