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
#include "node.h"
#include "utility.h"
#include "error.h"
#include "stack.h"
#include "set.h"
using namespace std;

/*
 * Reversing a Linked List (Code Writing)
 * --------------------------------------------------------
 * Write a function reverse that reverses the order of the
 * elements in a list of integers.
 */

void reverse(Node*& front) {
    Node* current = front;
    Node* previous = nullptr;
    while(current != nullptr){
        Node* next = current->next;
        current->next = previous;
        previous = current;
        current = next;
    }
    front = previous;
}


/* * * * * Provided Tests Below This Point * * * * */
PROVIDED_TEST("Example from handout"){
    Node *originalList = createListFromVector({1, 8, 19, 4, 17});
    Node *reversedList = createListFromVector({17, 4, 19, 8, 1});

    reverse(originalList);

    EXPECT(listEqual(originalList, reversedList));
}
