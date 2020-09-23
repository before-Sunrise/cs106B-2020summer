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
#include "error.h"
#include "vector.h"
using namespace std;

/*
 * Weights and Balances (Code Writing)
 * --------------------------------------------------------
 * Write a recursive function that determines whether it is
 * possible to measure out the desired target amount with a
 * given set of weights, which is stored in the vector
 * weights.(给定一个物体和一个秤砣的vector，判断是否能用这些秤砣秤这个物体（秤砣两边都可以放）
 */
bool isMeasurableHelper(int target, Vector<int> & weights, int sum, int index);

bool isMeasurable(int target, Vector<int>& weights){

    return isMeasurableHelper(target, weights, 0, 0);
}

// sum表示当前状态下的sum，index为下一步会处理的index
bool isMeasurableHelper(int target, Vector<int> & weights, int sum, int index){
    if(sum == target){
        return true;
    }else if(index == weights.size()){
        //下一步如果越过边界了，说明已处理完所有元素
        return false;
    }else return isMeasurableHelper(target, weights, sum - weights[index], index + 1) //下一元素放到左边
            || isMeasurableHelper(target, weights, sum, index + 1) //下一元素不使用
            || isMeasurableHelper(target, weights, sum + weights[index], index + 1); //下一元素放到右边
}

/* * * * * Provided Tests Below This Point * * * * */

PROVIDED_TEST("Provided Test: Simple Positive Example"){
    Vector<int> weights = {1, 3};
    EXPECT(isMeasurable(2, weights));
}

PROVIDED_TEST("Provided Test: Simple Negative Example"){
    Vector<int> weights = {1, 3};
    EXPECT(!isMeasurable(5, weights));
}

PROVIDED_TEST("Provided Test: Complex Positive Example"){
    Vector<int> weights = {1, 3, 7};
    EXPECT(isMeasurable(6, weights));
}
