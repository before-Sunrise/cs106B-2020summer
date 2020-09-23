/*
 * CS106B Section Handout Test Harness: Section 6
 * ----------------------------------------------
 * These problems have been galvanized from years of
 * section handouts that have been worked on by numerous
 * instructors and TA's. Codified by Trip Master and Nick
 * Bowman for CS106B Spring 2020.
 *
 * A huge thank you to Keith Schwarz and Julie Zelenski
 * for creating an amazing testing harness!
 */

#include <iostream>
#include "testing/SimpleTest.h"
#include "testing/TextUtils.h"
#include "testing/TestDriver.h"
#include "node.h"
#include "utility.h"
#include "error.h"
#include "set.h"
using namespace std;

/*
 * Doubling a List (Code Writing)
 * --------------------------------------------------------
 * Write a function that takes a pointer to the front of a
 * linked list of integers and appends a copy of the original
 * sequence to the end of the list. Do not use any auxiliary
 * data structures to solve this problem. You should only
 * construct one additional node for each element in the
 * original list. Your function should run in O(n) time where
 * n is the number of nodes in the list.
 */

void doubleList(Node*& front) {
    if(front == nullptr){
        error("empyt linklist");
    }
    Node *current = front, *previous;
    int length = 1;
    // 当前元素不是最后一个元素，则长度+1，指向下一元素
    while(current->next != nullptr){
        length++;
        current = current->next;
    }
    //current point to the last element
    previous = current;
    current = front;
    while(length > 0){
        Node *node = new Node;
        node->data = current->data;
        previous->next = node;
        length--;
        current = current->next;
        previous = node;
    }
    previous->next = nullptr;
}
/* official solution, very good
void doubleList(Node*& front) {
    if (front != nullptr) {
        Node *half2 = new Node(front->data, nullptr);
        Node *back = half2;
        Node *current = front;
        while (current->next != nullptr) {
            current = current->next;
            back->next = new Node(current->data, nullptr);
            back = back->next;
        }
        current->next = half2;
    }
}
*/
/* * * * * Provided Tests Below This Point * * * * */
PROVIDED_TEST("Example from handout"){
    Node *originalList = createListFromVector({1, 3, 2, 7});
    Node *doubled = createListFromVector({1, 3, 2, 7, 1, 3, 2, 7});

    doubleList(originalList);

    EXPECT(listEqual(originalList, doubled));

}
PROVIDED_TEST("size is one"){
    Node *originalList = createListFromVector({1});
    Node *doubled = createListFromVector({1, 1});

    doubleList(originalList);

    EXPECT(listEqual(originalList, doubled));

}

