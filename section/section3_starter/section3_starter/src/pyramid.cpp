/*
 * CS106B Section Handout Test Harness: Section 2
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
using namespace std;

/*
 * Human Pyramids (Code Writing)
 * --------------------------------------------------------
 * Your task is to write a function that takes as input the height
 * of the human pyramid (the number of layers it has) and returns
 * the number of people in that pyramid.
 */
// f(n) = (1+n)*n/2 = n^2/2 + n/2
// f(n-1) = (1+n-1)*(n-1)/2 = n*(n-1)/2 = n^2/2-n/2
// f(n) = f(n-1) + n;

int peopleInPyramidOfHeight(int n){
    if(n == 1) return 1;
    else{
        return  peopleInPyramidOfHeight(n - 1) + n;
    }
    return 0;
}


/* * * * * Provided Tests Below This Point * * * * */

PROVIDED_TEST("Provided Test: Example from handout.") {
    EXPECT_EQUAL(peopleInPyramidOfHeight(3), 6);
}

