#include "error.h"
#include "strlib.h"
#include "OurSet.h"
#include "testing/SimpleTest.h"

/*
 * Constructor for OurSet class. Intializes instance variables to
 * correct values
 */
OurSet::OurSet() {
    numItems = 0;
    root = nullptr;
}

/*
 * Destructor for OurSet class. Frees all the memory associated with
 * the internal binary search tree.
 */
OurSet::~OurSet(){
    freeTree(root);
}

/*
 * Frees all the memory associated with the nodes in the tree.
 * Helper function for the destructor and clear() functions.
 */
void OurSet::freeTree(TreeNode* tree) {
    if (tree == nullptr) {
        return;
    }
    freeTree(tree->left);
    freeTree(tree->right);
    delete tree;
}

/*
 * Adds the specified value to the set by inserting it into the correct
 * location in the internal binary search tree.
 */
void OurSet::add(string value) {
    addHelper(value, root);
}

/*
 * Recursive helper function to add the specified value into the tree
 * whose root is represented by the specified node.
 */
void OurSet::addHelper(string value, TreeNode*& node) {
    if (node == nullptr) {
        node = new TreeNode(value, nullptr, nullptr);
        numItems++;
    } else if (node->data > value) {
        addHelper(value, node->left);
    } else if (node->data < value) {
        addHelper(value, node->right);
    }
    // Do not add to set if value already exists in tree
}

/*
 * Checks if the specified value is in the set. Returns
 * true if the element is in the set, and false otherwise.
 */
bool OurSet::contains(string value) {
    return containsHelper(value, root);
}

/*
 * Recursive helper function to traverse the tree whose root value is represented
 * by node and see if the specified value is contained in the tree.
 */
bool OurSet::containsHelper(string value, TreeNode* node) {
    /*
     * Base Case 1: The current tree root is nullptr, which means the tree
     * we're exploring is empty. Since no value can be found in an empty tree
     * we always return false.
     */
    if (node == nullptr) {
        return false;
    }

    /*
     * Base Case 2: The current tree root contains the value we're looking for,
     * which means we've found our desired element.
     */
    if (node->data == value) {
        return true;
    }

    /*
     * Recursive Case:
     *
     * If the desired value is less than the current data point, explore
     * the left subtree. Otherwise, explore the right subtree.
     */
    if (value < node->data) {
        return containsHelper(value, node->left);
    } else {
        return containsHelper(value, node->right);
    }
}

/* Removes the specified value from the set by deleting the element
 * containing that value from the internal binary search tree, making sure
 * to preserve the sturcture of the BST.
 */
void OurSet::remove(string value){
    removeHelper(root, value);
}

/*
 * Recursive helper function to help with removing a specified value from the
 * tree whose root is represented by the provided node.
 */
void OurSet::removeHelper(TreeNode*& node, string value) {
    /* If the tree is empty, there’s nothing to remove! */
    if (node == nullptr) {
        return;
    }
    /* If the node to delete is to the left, remove it from there. */
    else if (value < node->data) {
        removeHelper(node->left, value);
    }
    /* If the node to delete is to the right, remove from there. */
    else if (value > node->data) {
        removeHelper(node->right, value);
    }
    /* Otherwise, we’ve found the node to remove – so go remove it! */
    else {
        performDeletion(node);
        numItems--;
    }
}

/* Actually does the deletion necessary to remove a node from the tree. */
void OurSet::performDeletion(TreeNode*& toRemove) {
    /* Case 1: The node is a leaf. Then we just delete it. */
    if (toRemove->left == nullptr && toRemove->right == nullptr) {
        delete toRemove;
        /* Inform whoever was pointing at us that we no longer exist. */
        toRemove = nullptr;
    }
    /* Case 2a: Only have a left child. */
    else if (toRemove->right == nullptr) {
        TreeNode* replacement = toRemove->left;
        delete toRemove;
        toRemove = replacement;
    }
    /* Case 2b: Only have a right child. */
    else if (toRemove->left == nullptr) {
        TreeNode* replacement = toRemove->right;
        delete toRemove;
        toRemove = replacement;
    }
    /* Case 3: Replace this node with the largest node in its left subtree. */
    else {
        toRemove->data = removeLargestFrom(toRemove->left);
    }
}

/* Deletes the largest value from the specified tree, returning that value. */
string OurSet::removeLargestFrom(TreeNode*& node) {
    if (node->right == nullptr) {
        string result = node->data;
        performDeletion(node);
        return result;
    }
    return removeLargestFrom(node->right);
}

/*
 * Remove all elements from the set.
 */
void OurSet::clear() {
    freeTree(root);

    // Reset member variables to default values
    root = nullptr;
    numItems = 0;
}

/*
 * Return the number of elements in the set
 */
int OurSet::size() {
    return numItems;
}

/*
 * Returns true if the set is empty, false otherwise.
 */
bool OurSet::isEmpty() {
    return (numItems == 0);
}

/*
 * Do an in-order traversal of the tree to display the contents
 * of the set in alphabetical order.
 *
 * Helpful as a debugging tool.
 */
void OurSet::printSetContents() {
    cout << "{";
    inorderPrintTree(root);
    cout << "}" << endl;
}

/* 
 * Helper function for printing the contents of our set in order.
 */
void OurSet::inorderPrintTree(TreeNode* tree) {
    if (tree == nullptr) {
        return;
    }
    inorderPrintTree(tree->left);
    cout << tree->data << " ";
    inorderPrintTree(tree->right);
}


PROVIDED_TEST("Test OurSet constructor and destructor."){
    OurSet ss;
    EXPECT(ss.isEmpty());
    EXPECT_EQUAL(ss.size(), 0);
}

PROVIDED_TEST("Test add and contains functions."){
    OurSet ss;

    /* Test add function to add some elements to the set. */
    ss.add("cat");
    EXPECT(!ss.isEmpty());
    EXPECT_EQUAL(ss.size(), 1);

    ss.add("dog");
    EXPECT_EQUAL(ss.size(), 2);

    Vector<string> animals = {"fish", "zebra", "aardvark", "lion", "meerkat", "marmot", "bird", "eagle"};
    for (string animal: animals){
        ss.add(animal);
    }
    EXPECT_EQUAL(ss.size(), 10);

    /* Display current state of the set. */
    cout << endl << "Full set: ";
    ss.printSetContents();

    /* Test contains function to see if add operations were successful. */
    EXPECT(ss.contains("cat"));
    EXPECT(ss.contains("dog"));
    EXPECT(ss.contains("fish"));
    EXPECT(ss.contains("marmot"));

    EXPECT(!ss.contains("hamster"));
    EXPECT(!ss.contains("platypus"));
}

PROVIDED_TEST("Test remove function"){
    OurSet ss;
    Vector<string> animals = {"cat", "dog", "fish", "zebra", "aardvark", "lion", "meerkat", "marmot", "bird", "eagle"};
    for (string animal: animals){
        ss.add(animal);
    }
    EXPECT_EQUAL(ss.size(), 10);

    /* Display current state of the set. */
    cout << endl << "Full set: ";
    ss.printSetContents();

    ss.remove("bird");
    EXPECT(!ss.contains("bird"));
    EXPECT_EQUAL(ss.size(), 9);

    cout << "New set contents: ";
    ss.printSetContents();

    ss.remove("lion");
    EXPECT(!ss.contains("lion"));
    EXPECT_EQUAL(ss.size(), 8);

    cout << "New set contents: ";
    ss.printSetContents();

    ss.remove("fish");
    EXPECT(!ss.contains("fish"));
    EXPECT_EQUAL(ss.size(), 7);

    cout << "New set contents: ";
    ss.printSetContents();

    /* Test removal of the root node. */
    ss.remove("cat");
    EXPECT(!ss.contains("cat"));
    EXPECT_EQUAL(ss.size(), 6);

    cout << "New set contents: ";
    ss.printSetContents();
}
