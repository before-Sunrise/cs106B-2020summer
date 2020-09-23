/**
 * File: warmup.cpp
 * ----------------
 * @author Julie Zelenski, Spring 2020
 * This code is provided as part of an exercise to practice using a
 * debugger. This program has buggy use of recursion.
 */

#include <iostream>    // for cout, endl
#include <string>      // for string class
#include "simpio.h"    // for getLine
#include "hanoigui.h"
#include "testing/SimpleTest.h"
using namespace std;


/*
 * This is a correct implementation of a function that uses recursion
 * to explore all possible subsets of elements from v and returns the
 * count of how many of those subsets sum to zero.
 */
int countZeroSumSubsets(Vector<int> &v, int index, int sumSoFar) {
    if (index == v.size()) {
        return (sumSoFar == 0);
    } else {
        return countZeroSumSubsets(v, index + 1, sumSoFar + v[index])
             + countZeroSumSubsets(v, index + 1, sumSoFar);
    }
}

/*
 * This function is only one character different than the
 * correct version above, but even a small edit can cause big havoc
 * when amplified by recursion.
 *
 * WARNING: The given code is buggy. See the exercise writeup for
 * more information on how to test and diagnose.
 */
int buggyCount(Vector<int> &v, int index, int sumSoFar) {
    if (index == v.size()) {
        return (sumSoFar == 0);
    } else {
        return buggyCount(v, index + 1, sumSoFar += v[index])
             + buggyCount(v, index + 1, sumSoFar);
    }
}

/* This function moves the given number of discs from the given starting
 * peg to the given ending peg using specified temporary peg.
 *
 * This function uses the functions of the HanoiGui to visually animate
 * the movement.
 *
 * This function also counts the total number of moves made in the given
 * int reference.
 */
void moveTower(int numDiscs, string startPeg, string endPeg, string tempPeg, int& totalMoves) {
    if (numDiscs == 1) {
        // base case: just move a single disc
        HanoiGui::moveOneDisc(startPeg, endPeg);
        totalMoves++;
    } else {
        // recursive case: move rest of tower recursively, then
        // we will handle the bottom (largest) peg ourselves
        moveTower(numDiscs - 1, startPeg, tempPeg, endPeg, totalMoves);
        HanoiGui::moveOneDisc(startPeg, endPeg);
        totalMoves++;
        moveTower(numDiscs - 1, tempPeg, endPeg, startPeg, totalMoves);
    }
}

/*
 * Thus function initializes the HanoiGui for the proper number of discs
 * and then recursively moves a tower from peg to peg.
 */
void hanoiAnimation() {
    int numDiscs = 4;
    HanoiGui::initialize(numDiscs);
    pause(1000);

    // do the recursive search and count total moves by reference
    int totalMoves = 0;
    moveTower(numDiscs, "left", "right", "middle", totalMoves);
    cout << "Total moves made = " << totalMoves << endl;
}

/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("Animate Towers of Hanoi") {
    hanoiAnimation();
}

PROVIDED_TEST("Test correct countZeroSumSubsets against buggyCount") {
    Vector<int> nums = {};
    EXPECT_EQUAL(countZeroSumSubsets(nums, 0, 0), buggyCount(nums, 0, 0));

    nums = {1, 2, 3, -4, -3, 2, 5, 1, 9, -2};
    EXPECT_EQUAL(countZeroSumSubsets(nums, 0, 0), buggyCount(nums, 0, 0));
}

STUDENT_TEST("minimal Test case that expouse the bug on buggyCount"){
    Vector<int> nums = {-1, 1};
    EXPECT_EQUAL(countZeroSumSubsets(nums, 0, 0), buggyCount(nums, 0, 0));

}
