#include <iostream>
#include "console.h"
#include "testing/SimpleTest.h"
#include "simpio.h"
#include "linkedlist.h"
using namespace std;

int main() {
    if (runSimpleTests(SELECTED_TESTS)){
        return 0;
    }

    Node* list1 = createListWithAppend({"Nick", "Kylie", "Trip"});
    cout << "List 1 has " << lengthOf(list1) << " elements in it!" << endl;
    printList(list1);
    freeList(list1);

    cout << endl << endl;
    Node* list2 = createListWithTailPtr({"Lions", "Tigers", "Bears", "Oh My!"});
    cout << "List 2 has " << lengthOf(list2) << " elements in it!" << endl;
    printList(list2);
    freeList(list2);

    cout << endl << endl;
    Node* head = createListWithTailPtr({"Bananas", "Dragonfruit", "Yuzu"});
    cout << "State of the list initially: " << endl;
    printList(head);
    cout << endl;

    /* Insert into middle of list. */
    alphabeticalAdd(head, "Oranges");
    alphabeticalAdd(head, "Coconuts");

    /* Insert into beginning and end of list. */
    alphabeticalAdd(head, "Apricots");
    alphabeticalAdd(head, "Zucchini"); /* Technically a fruit! */

    cout << "State of the list after insertions: " << endl;
    printList(head);
    cout << endl;


    /* Remove from the middle of the list. */
    remove(head, "Oranges");
    remove(head, "Coconuts");

    /* Remove from the beginning and end of the list. */
    remove(head, "Apricots");
    remove(head, "Zucchini");

    cout << "State of the list after removals: " << endl;

    printList(head);
    freeList(head);

    return 0;
}

PROVIDED_TEST("Timing Test to compare appending speed with and without tail pointer"){
    int startSize = 10000;
    for (int size = startSize; size < 10 * startSize; size *= 2){
        Vector<string> vals(size);
        Node* list1;
        TIME_OPERATION(size, list1 = createListWithAppend(vals));
        Node* list2;
        TIME_OPERATION(size, list2 = createListWithTailPtr(vals));
        freeList(list1);
        freeList(list2);
    }
}




