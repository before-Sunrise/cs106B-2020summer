#pragma once

#include "node.h"
#include "vector.h"

Node* createListFromVector(Vector<int> nums);
void freeList(Node* list);
bool listEqual(Node* a, Node *b);
void printList(Node *list);

StringNode* createStringListFromVector(Vector<std::string> vals);
void freeStringList(StringNode* list);
bool stringListEqual(StringNode* a, StringNode *b);
void printStringList(StringNode* list);

DoubleNode* createDoubleListFromVector(Vector<double> nums);
void freeDoubleList(DoubleNode* list);
bool doubleListEqual(DoubleNode* a, DoubleNode *b);
void printDoubleList(DoubleNode* list);

