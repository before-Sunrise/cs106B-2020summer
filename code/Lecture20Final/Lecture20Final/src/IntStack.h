#pragma once

struct Node {
    int data;
    Node* next;
};

class IntStack {
public:
    IntStack(); // constructor
    ~IntStack();
    
    void push(int value);
    int pop();
    bool isEmpty();

private:    
    Node* top;
};
