#pragma once
#include <string>

struct Node {
    int data;
    Node *next;
};

struct StringNode {
    std::string data;
    StringNode *next;
};

struct DoubleNode {
    double data;
    DoubleNode *next;
};
