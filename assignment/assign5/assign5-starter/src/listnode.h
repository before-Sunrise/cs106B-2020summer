#pragma once

#include "testing/MemoryDiagnostics.h"

struct ListNode {
    int data;           /* Data stored in the node. */
    ListNode* next;     /* Pointer to next node in the list. */

    /*
     * This is the default ListNode constructor. Ths constructor
     * gets called when the following line of code is executed.
     *
     * ListNode* node = new ListNode;
     *
     * This version of the constructor does NOT initialize either of
     * the two field values, which can lead to undefined behavior
     * if not used carefully!
     */
    ListNode() {}

    /*
     * This is the two-argument version of the ListNode constructor.
     * The constructor gets called when the following line of code is
     * executed:
     *
     * ListNode* node = new ListNode(10, nullptr);
     *
     * This version of the constructor initializes both the data and next
     * pointer fields to the specified values.
     */
    ListNode(int d, ListNode* n) {
        data = d;
        next = n;
    }

    TRACK_ALLOCATIONS_OF(ListNode); /* Track allocation counts in test cases. */
};
