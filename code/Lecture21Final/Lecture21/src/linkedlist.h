#include <iostream>
#include <string>
#include "simpio.h"
#include "console.h"

/* Type: Node
 *
 * A node in a linked list.
 */
struct Node {
    std::string data;
    Node* next;

    /*
     * This is the default Node constructor. Ths constructor
     * gets called when the following line of code is executed.
     *
     * Node* node = new Node;
     *
     * This version of the constructor does NOT initialize either of
     * the two field values, which can lead to undefined behavior
     * if not used carefully!
     */
    Node() {}

    /*
     * This is the two-argument version of the Node constructor.
     * The constructor gets called when the following line of code is
     * executed:
     *
     * Node* node = new Node(10, nullptr);
     *
     * This version of the constructor initializes both the data and next
     * pointer fields to the specified values.
     */
    Node(std::string d, Node* n) {
        data = d;
        next = n;
    }
};

/* Given a linked list, returns the length of that list. */
int lengthOf(Node* list);

/* Prints the contents of a linked list, in order. */
void printList(Node* list);

/* Frees all the memory used by a linked list. */
void freeList(Node* list);

/* Reads a linked list from the user one element at a time, stopping when
 * the user enters an empty string. This returns the list, constructed in
 * reverse order.
 */
Node* readList();

/* Given a linked list, returns the length of that list. Operates recursively. */
int lengthOfRec(Node* list);

/* Prints the contents of a linked list, in order. Operates recursively. */
void printListRec(Node* list);

/* Frees all the memory used by a linked list. Operates recursively. */
void freeListRec(Node* list);

/* Add a new element containing the provided data to the beginning of the specified
 * list. */
void prependTo(Node*& list, std::string data);

/* Adds a new element containing the provided data to the end of the specified list. */
void appendTo(Node*& list, std::string data);
