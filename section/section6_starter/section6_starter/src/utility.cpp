#include "utility.h"

Node* createListFromVector(Vector<int> nums){
	if (nums.isEmpty()) return nullptr;
	Node *head = new Node;
	head->data = nums[0];
	head->next  = nullptr;

	Node *cur = head;
	for (int i = 1; i < nums.size(); i++){
		Node *newNode = new Node;
		newNode->data = nums[i];
		newNode->next = nullptr;
		cur->next = newNode;
		cur = newNode;
	}	
	return head;
}

void freeList(Node* list){
	if (list == nullptr) return;
	freeList(list->next);
	delete list;
}

bool listEqual(Node *a, Node *b){
    while (a != nullptr){
        if (b == nullptr || a->data != b->data) return false;
        a = a->next;
        b = b->next;
    }
    // make sure lists are the same length
    return a == nullptr && b == nullptr;
}

/* Prints the contents of a linked list, in order. */
void printList(Node* list) {
    for (Node* cur = list; cur != nullptr; cur = cur->next) {
        std::cout << cur->data << std::endl;
    }
}

StringNode* createStringListFromVector(Vector<std::string> vals){
    if (vals.isEmpty()) return nullptr;
    StringNode *head = new StringNode;
    head->data = vals[0];
    head->next  = nullptr;

    StringNode *cur = head;
    for (int i = 1; i < vals.size(); i++){
        StringNode *newNode = new StringNode;
        newNode->data = vals[i];
        newNode->next = nullptr;
        cur->next = newNode;
        cur = newNode;
    }
    return head;
}

void freeStringList(StringNode* list){
    if (list == nullptr) return;
    freeStringList(list->next);
    delete list;
}

bool stringListEqual(StringNode* a, StringNode *b){
    while (a != nullptr){
        if (b == nullptr || a->data != b->data) return false;
        a = a->next;
        b = b->next;
    }
    // make sure lists are the same length
    return a == nullptr && b == nullptr;
}

/* Prints the contents of a linked list, in order. */
void printStringList(StringNode* list) {
    for (StringNode* cur = list; cur != nullptr; cur = cur->next) {
        std::cout << cur->data << std::endl;
    }
}

DoubleNode* createDoubleListFromVector(Vector<double> nums){
    if (nums.isEmpty()) return nullptr;
    DoubleNode *head = new DoubleNode;
    head->data = nums[0];
    head->next  = nullptr;

    DoubleNode *cur = head;
    for (int i = 1; i < nums.size(); i++){
        DoubleNode *newNode = new DoubleNode;
        newNode->data = nums[i];
        newNode->next = nullptr;
        cur->next = newNode;
        cur = newNode;
    }
    return head;
}

void freeDoubleList(DoubleNode* list){
    if (list == nullptr) return;
    freeDoubleList(list->next);
    delete list;
}

bool doubleListEqual(DoubleNode* a, DoubleNode *b){
    while (a != nullptr){
        if (b == nullptr || a->data != b->data) return false;
        a = a->next;
        b = b->next;
    }
    // make sure lists are the same length
    return a == nullptr && b == nullptr;
}

/* Prints the contents of a linked list, in order. */
void printDoubleList(DoubleNode* list) {
    for (DoubleNode* cur = list; cur != nullptr; cur = cur->next) {
        std::cout << cur->data << std::endl;
    }
}


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
    std::cout << root->data << " ";
    printTree(root->left);
    printTree(root->right);
}
