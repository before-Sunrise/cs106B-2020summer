/*
 * CS106B Section Handout Test Harness: Section 3
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
#include "vector.h"
using namespace std;

/*
 * Win some, lose sum (Code Writing)
 * --------------------------------------------------------
 * Write a recursive function named canMakeSum that takes a
 * reference to a Vector<int> and an int target value and
 * returns true if it is possible to have some selection of
 * values from the Vector that sum to the target value.
 */

bool canMakeSumHelper(Vector<int>& values, int target, int index, int sum);

// recursive backtracking
bool canMakeSum(Vector<int>& values, int target) {

    return canMakeSumHelper(values, target, 0, 0);
}

bool canMakeSumHelper(Vector<int>& values, int target, int index, int sum){
    if(target == sum){
         return true;
    }
    else if(index == values.size()){
        return false;
    }
    else if(sum > target){
        return false;
    }
    else{
        int number = values[index];
        return canMakeSumHelper(values, target,  index + 1, sum + number)
                || canMakeSumHelper(values, target, index + 1, sum);

    }
}
/* * * * * Provided Tests Below This Point * * * * */

PROVIDED_TEST("Provided Test: Positive example from handout.") {
    Vector<int> nums = {1,1,2,3,5};
    EXPECT(canMakeSum(nums, 9));
}

PROVIDED_TEST("Provided Test: Negative example from handout"){
    Vector<int> nums = {1,4,5,6};
    EXPECT(!canMakeSum(nums, 8));
}
