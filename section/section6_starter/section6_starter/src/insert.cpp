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
#include <string>
#include "testing/SimpleTest.h"
#include "testing/TextUtils.h"
#include "node.h"
#include "utility.h"
#include "error.h"
using namespace std;

/*
 * Inserting into a Linked List (Code Writing)
 * --------------------------------------------------------
 * Write a function named insert that accepts a reference to a
 * StringNode pointer representing the front of a linked list,
 * along with an index and a string value. Your function should
 * insert the given value into a new node at the specified
 * position of the list.
 *
 * The other values in the list should retain
 * the same order as in the original list. You may assume that the
 * index passed is between 0 and the existing size of the list,
 * inclusive.
 *
 * Constraints: Do not modify the data field of existing nodes;
 * change the list by changing pointers only. Do not use any
 * auxiliary data structures to solve this problem (no array,
 * Vector, Stack, Queue, string, etc).
 */

//插入到第index个位置，必须先找到第index-1个位置，如果第index-1个位置不存在则error
void insert(StringNode*& front, int index, string value) {
    if(index < 0){
        error("index can't be negative");
    }

    StringNode* previous;
    int count;

    StringNode* Node = new StringNode;
    Node->data = value;

    if(index == 0){
        Node->next = front;
        front = Node;
        return;
    }

    //如果index不为0，传入一个空链表，报错
    if(front == nullptr){
        error("can't insert an empty linklist when index is not 0");
    }
    //此时链表不为空，将previous指向第一个元素
    previous = front;
    count = 0;
    //在previous没指到第index-1个且previous后面有元素时
    while(count <  index - 1 && previous->next != nullptr){
        //previous指向后一个
        previous = previous->next;
        count++;
    }
    // previous没指到第index-1个元素
    if(count != index -1){
        error("index is too big");
    }
    StringNode* next = previous->next;
    previous->next = Node;
    Node->next = next;
}

/* * * * * Provided Tests Below This Point * * * * */
PROVIDED_TEST("Example from handout"){
    StringNode *originalList = createStringListFromVector({ "Katherine", "Julie", "Kate" });
    StringNode *solnList = createStringListFromVector({ "Katherine", "Julie", "Mehran", "Kate" });

    insert(originalList, 2, "Mehran");
    EXPECT(stringListEqual(solnList, originalList));
}

STUDENT_TEST("empyty list"){
    StringNode* originalList = nullptr;
    StringNode* solnList = createStringListFromVector({"jiang"});

    insert(originalList, 0, "jiang");
    EXPECT(stringListEqual(solnList, originalList));
}

STUDENT_TEST("insert element into an empyty list when index is bigger than 0"){
    StringNode* originalList = nullptr;
    EXPECT_ERROR(insert(originalList, 2, "jiang"));
}

STUDENT_TEST("insert element into the end of the list"){
    StringNode* originalList = createStringListFromVector({ "Katherine", "Julie", "Kate" });
    StringNode *solnList = createStringListFromVector({ "Katherine", "Julie", "Kate", "jiang" });
    insert(originalList, 3, "jiang");
    EXPECT(stringListEqual(solnList, originalList));
}

STUDENT_TEST("insert element into a list which is not empty when index is out of the boundary"){
    StringNode* originalList = createStringListFromVector({ "Katherine", "Julie", "Kate" });
    EXPECT_ERROR(insert(originalList, 4, "jiang"));
    EXPECT_ERROR(insert(originalList, -1, "jiang"));
}

