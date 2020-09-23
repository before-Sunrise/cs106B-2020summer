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

    Node* list = readList();

    cout << "This list contains " << lengthOf(list) << " cells." << endl;
    printList(list);
    freeList(list);

    cout << endl << endl;

    Node* head = nullptr;

    appendTo(head, "Trip");
    appendTo(head, "Nick");
    appendTo(head, "Kylie");

    printList(head);
    freeList(head);

    return 0;
}




