#include "listnode.h"
#include "random.h"
#include "testing/SimpleTest.h"
using namespace std;

/*
 * This function is a buggy implementation of freeing a
 * linked list. In particular, on every iteration of the while
 * loop, the function attempts to access memory that was just
 * deallocated.
 */
void badDeallocate(ListNode* list) {
    while (list != nullptr) {
        delete list;
        list = list->next;  // BAD: accesses deallocated memory
    }
}

//PROVIDED_TEST("Allocation counts: one new, no delete") {
//    ListNode* single = new ListNode(5, nullptr);
//    EXPECT_EQUAL(single->data, 5);
//}

//PROVIDED_TEST("Allocation counts: 10 new, one delete") {
//    ListNode* list = nullptr;
//    for (int i = 0; i < 10; i++) {
//        list = new ListNode(i, list);
//    }
//    delete list;
//    EXPECT_EQUAL(list->data, 9);    // BAD: accesses deallocated memory
//}

//PROVIDED_TEST("Allocation counts: 2 new, 2 delete") {
//    ListNode* first = new ListNode(5, nullptr);
//    ListNode* second = new ListNode(8, nullptr);
//    first = second; // first and second are now aliases
//    EXPECT_EQUAL(first, second);
//    delete first;
//    delete second;
//}

//PROVIDED_TEST("badDeallocate: list of length 3") {
//    ListNode* list = nullptr;
//    for (int i = 0; i < 3; i++) {
//        list = new ListNode(i, list);
//    }
//    badDeallocate(list);
//}

//PROVIDED_TEST("badDeallocate: longer lists") {
//    for (int n = 0; n < 5000; n++) { // change number of trials to explore
//        int size = randomInteger(6000, 9000); // make long list
//        ListNode* list = nullptr;
//        for (int i = 0; i < size; i++) {
//            list = new ListNode(i, list);
//        }
        
//        if (randomChance(.50)) {    // leak about half of lists
//            badDeallocate(list);
//        }
//    }
//}

/*
 * Buggy test that fails to properly initialize variables.
 * What are the consequences of using a garbage integer?
 * What are the consequences of using a garbage pointer?
 */
PROVIDED_TEST("Segmentation fault: use of uninitialized values") {
    ListNode* p = new ListNode; // allocate new node, don't assign data or next field, leave uninitialized

    p->data += 10;   // add 10 to garbage, what happens?

    p->next->data = 5; // dereference garbage address, what happens?

    delete p;
}
