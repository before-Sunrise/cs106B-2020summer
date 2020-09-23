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
