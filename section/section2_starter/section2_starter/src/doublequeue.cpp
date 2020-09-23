/*
 * CS106B Section Handout Test Harness: Section 1
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
#include "queue.h"
#include "testing/SimpleTest.h"
#include "testing/TextUtils.h"
using namespace std;

/*
 * DoubleQueue (Code Write)
 * ----------------------------------
 * Write a function that takes in a queue and replaces every
 * element with two copies of itself.
 */

void doubleQueue(Queue<int>& q) {
    int length = q.size(),elem;
    for(int i = 0; i < length; i++){
        elem = q.dequeue();
        q.enqueue(elem);
        q.enqueue(elem);
    }

}


/* * * * * Provided Tests Below This Point * * * * */

PROVIDED_TEST("test case from handout") {
    Queue<int> q = {1, 2, 3};
    Queue<int> soln = {1, 1, 2, 2, 3, 3};

    doubleQueue(q);

    EXPECT_EQUAL(q, soln);
}
