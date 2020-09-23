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
#include "vector.h"
#include "strlib.h"
#include "testing/SimpleTest.h"
#include "testing/TextUtils.h"
using namespace std;

/*
 * Remove -Consecutive Duplicates (Debugging)
 * -------------------------------------------------------
 * Below is a buggy implementation of a function that is supposed to remove
 * consecutive duplicates from a vector. Unfortunately there are 3 bugs. Can you find
 * them and fix the code so that it passes all of the tests?
 */

/*
void deduplicate(Vector<string> vec) {
    for (int i = 0; i < vec.size(); i++) {
        if (vec[i] == vec[i + 1]) {
            vec.remove(i);
        }
    }
}
*/

// my answer(passed the test, but is very ugly
void deduplicate(Vector<string>& vec) {
    if(vec.isEmpty())
        return;
    int index = 0;
    if(index == vec.size()-1) return; //vec has only one element
    while(true){
        if(index == vec.size()-1) break; //after the index++, index may point to the last element of vec
        if(vec[index] == vec[index+1]){
            vec.remove(index+1);
        }
        if(index == vec.size()-1) break;// after the remove, index may point to the last element of vec
        if(vec[index] != vec[index+1]){
            index++;
        }
    }
}

/*
There are three errors here:

Calling .remove() on the Vector while iterating over it doesn’t work particularly nicely. Specifically, if you remove the element at index i and then increment i in the for loop, you’ll skip over the element that shifted into the position you were previously in.
There’s an off-by-one error here: when i = vec.size() - 1, the indexing vec[i + 1] reads off the end of the Vector.
The Vector is passed in by value, not by reference, so none of the changes made to it will persist to the caller.
Here’s a corrected version of the code:

void deduplicate(Vector<string>& vec) {
    for (int i = 0; i < vec.size() - 1; ) {
        if (vec[i] == vec[i + 1]) {
            vec.remove(i);
        } else {
            i++;
        }
    }
}
*/

/* * * * * Provided Tests Below This Point * * * * */

PROVIDED_TEST("Provided Test: Verifies our solution works on the empty vector.") {
    Vector<string> v = {};
    Vector<string> empty = {};
    deduplicate(v);
    EXPECT_EQUAL(v, empty);
}

PROVIDED_TEST("Provided Test: Verifies removeDuplicates works on a simple vector.") {
    Vector<string> v = {"Katherine Johnson",
                        "Katherine Johnson",
                        "Katherine Johnson",
                        "Mary Jackson",
                        "Dorothy Vaughan",
                        "Dorothy Vaughan"};
    Vector<string> vNoDup = {"Katherine Johnson", "Mary Jackson", "Dorothy Vaughan"};
    deduplicate(v);
    EXPECT_EQUAL(v, vNoDup);
}

#include "random.h"
PROVIDED_TEST("Provided Test: STRESS TEST - Verifies removeDuplicates works on a large vector") {
    const int K_NUM_MAX = 10000;
    Vector<string> largeVec;
    for (size_t i = 0; i < K_NUM_MAX; i++) {
        int numRepeats = randomInteger(1, 5);
        for (int j = 0; j < numRepeats; j++) {
            largeVec += integerToString(i);
        }
    }
    deduplicate(largeVec);
    for (size_t i = 0; i < K_NUM_MAX; i++) {
        EXPECT_EQUAL(largeVec[i], integerToString(i));
    }
}


