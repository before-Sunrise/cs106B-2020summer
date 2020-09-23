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
#include "vector.h"
using namespace std;

/*
 * Linked List Warmup (Code Writing)
 * --------------------------------------------------------
 * Write a function that adds up the values of all the elements
 * in the linked list. Next, write a function that returns a
 * pointer to the last element of a linked list (and reports an
 * error if the list is empty).
 */

int sumOfElementsIn(Node* list){
    if(list == nullptr){
        return 0;
    }
    int result = 0;
    while(list != nullptr){
        result += list->data;
        list = list->next;
    }
    return result;
}

int sumOfElementsInRecursively(Node* list){
    if(list == nullptr){
        return 0;
    }
    if(list->next == nullptr){
        return list->data;
    }
    return list->data + sumOfElementsInRecursively(list->next);
}

Node* lastElementOf(Node* list){
    if(list == nullptr){
        error("can't pass an empty NodeList");
    }
    while (list->next != nullptr) {
        list = list->next;
    }
    return list;
}

Node* lastElementOfRecursively(Node* list){
    if(list == nullptr){
        error("can't pass an empty Nodelist");
    }
    if(list->next == nullptr){
        return list;
    }else{
        return lastElementOfRecursively(list->next);
    }
}


/* * * * * Provided Tests Below This Point * * * * */

PROVIDED_TEST("Basic Sum Example"){
    Node *list = createListFromVector({1, 2, 3, 4, 5});
    EXPECT_EQUAL(sumOfElementsIn(list), 15);
    freeList(list);

    list = nullptr;
    EXPECT_EQUAL(sumOfElementsInRecursively(list), 0);
}

STUDENT_TEST("Sum Example by recursive method"){
    Node *list = createListFromVector({1, 2, 3, 4, 5});
    EXPECT_EQUAL(sumOfElementsInRecursively(list), 15);
    freeList(list);
}

PROVIDED_TEST("Basic Find Last Example"){
    Node *list = createListFromVector({1,2,3});
    EXPECT_EQUAL(lastElementOf(list), list->next->next);
    freeList(list);
}

STUDENT_TEST("Find Last Example recursively"){
    Node *list = createListFromVector({1,2,3});
    EXPECT_EQUAL(lastElementOf(list), list->next->next);
    freeList(list);

    list = nullptr;
    EXPECT_ERROR(lastElementOfRecursively(list));


}


