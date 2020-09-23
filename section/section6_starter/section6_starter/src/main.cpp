#include <iostream>
#include "console.h"
#include "testing/SimpleTest.h"

using namespace std;


int main() 
{
    if (runSimpleTests(SELECTED_TESTS)) {
        return 0;
    }
    return 0;
}

struct Node{
    int data;
    Node* next;
};

void reverseList(Node* &list){
    Node* point = list;
    Node* previous = nullptr;
    if(list == nullptr){
        return;
    }
    while(point != nullptr){
        Node* temp = point->next; // record the next element in original list
        point->next = previous; //
        previous = point;
        point = temp; //let point now change to next element in original llist
    }
    // now point is the final element in original list
    list = previous;
}
