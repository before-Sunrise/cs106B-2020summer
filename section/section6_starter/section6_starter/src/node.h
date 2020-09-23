#pragma once
#include <string>

struct Node {
    int data;
    Node *next;

    // default constructor does not initialize
    Node() {}

    // 2-arg constructor sets fields from arguments
    Node(int d, Node* n) {
        data = d;
        next = n;
    }
};

struct StringNode {
    std::string data;
    StringNode *next;

    // default constructor does not initialize
    StringNode() {}

    // 2-arg constructor sets fields from arguments
    StringNode(std::string s, StringNode* n) {
        data = s;
        next = n;
    }
};

struct DoubleNode {
    double data;
    DoubleNode *next;

    // default constructor does not initialize
    DoubleNode() {}

    // 2-arg constructor sets fields from arguments
    DoubleNode(double d, DoubleNode* n) {
        data = d;
        next = n;
    }
};
