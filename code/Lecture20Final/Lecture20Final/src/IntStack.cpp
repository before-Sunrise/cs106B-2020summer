#include "IntStack.h"
#include "testing/SimpleTest.h"

IntStack::IntStack() {
    top = nullptr;
}

IntStack::~IntStack() {
    while (top != nullptr) {
        Node *temp = top;
        top = top->next;
        delete temp;
    }
}

void IntStack::push(int value) {
    Node* temp = new Node;

    temp->data = value;
    temp->next = top;

    top = temp;
}

int IntStack::pop() {
    if (isEmpty()) {
        throw "Error! Trying to pop from empty stack!";
    }

    int topValue = top->data;

    Node* temp = top;
    top = top->next;

    delete temp;
    return topValue;
}

bool IntStack::isEmpty(){
    return top == nullptr;
}


PROVIDED_TEST("Reverse simple set of values"){
    IntStack stack;

    for (int i = 0; i < 10; i++){
        stack.push(i);
    }

    for (int i = 9; i >= 0; i--){
        EXPECT_EQUAL(i, stack.pop());
    }
}
