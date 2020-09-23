/*
 * This file contains supporting code that we used to help
 * us with sorting.
 * 
 * We did not write this code in class.
 */

#include "random.h"
#include "utility.h"
using namespace std;

/*
 * Returns true if the given vector's elements are in sorted order.
 */
bool isSorted(const Vector<int>& v) {
    for (int i = 1; i < v.size(); i++) {
        if (v[i] < v[i - 1]) {
            return false;
        }
    }
    return true;
}

/*
 * Fills the given vector with the given number of random integers.
 */
void fillRandomIntVector(Vector<int>& v, int length) {
    for (int i = 0; i < length; i++) {
        v += randomInteger(0, 1000000000);
    }
}

/*
 * Fills the given vector with the given number of sorted integers.
 */
void fillSortedIntVector(Vector<int>& v, int length) {
    v.add(randomInteger(0, 10));
    for (int i = 1; i < length; i++) {
        // add another value slightly larger than the last
        v += v[i-1] + randomInteger(0, 10);
    }
}

/*
 * Moves the value at index i to index j, and vice versa.
 */
void swap(Vector<int>& v, int i, int j) {
    int temp = v[i];
    v[i] = v[j];
    v[j] = temp;
}
