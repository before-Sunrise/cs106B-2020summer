/**
 * This program contains code for various sorting algorithms
 * including insertion sort, selection sort,
 * recursive merge sort, and recursive quick sort.
 */

#include <fstream>
#include <iomanip>
#include <iostream>
#include "console.h"
#include "simpio.h"
#include "filelib.h"
#include "random.h"
#include "vector.h"
#include "timer.h"
#include "utility.h"
#include "shuffle.h"
#include "testing/SimpleTest.h"

using namespace std;

// function prototype declarations
int indexOfSmallest(const Vector<int>& elems, int startPoint);
void selectionSort(Vector<int>& v);
void insertionSort(Vector<int>& v);
void mergeSort(Vector<int>& v);
void split(Vector<int>& vec, Vector<int>& left, Vector<int>& right);
void merge(Vector<int>& vec, Vector<int>& v1, Vector<int>& v2);
void quickSort(Vector<int>& vec);
void partition(Vector<int>& vec, Vector<int>& less, Vector<int>& equal, Vector<int>& greater, int pivot);
void concatenate(Vector<int>& vec, Vector<int>& less, Vector<int>& equal, Vector<int>& greater);
void swap(Vector<int>& v, int i, int j);

/*
 * Rearranges the elements of v into sorted order using
 * the selection sort algorithm.
 */
void selectionSort(Vector<int>& elems) {
    for (int index = 0; index < elems.size(); index++) {
        int smallestIndex = indexOfSmallest(elems, index);
        swap(elems, index, smallestIndex);
    }
}

/*
 * Given a vector and a starting point, returns the index of the smallest
 * element in that vector at or after the starting point
 */
int indexOfSmallest(const Vector<int>& elems, int startPoint) {
    int smallestIndex = startPoint;
    for (int i = startPoint + 1; i < elems.size(); i++) {
        if (elems[i] < elems[smallestIndex]) {
            smallestIndex = i;
        }
    }
    return smallestIndex;
}


/*
 * Rearranges the elements of v into sorted order using
 * the insertion sort algorithm.
 */
void insertionSort(Vector<int>& v) {
    for (int i = 0; i < v.size(); i++) {
        /* Scan backwards until either (1) the preceding
         * element is no bigger than us or (2) there is
         * no preceding element. */
        for (int j = i - 1; j >= 0; j--) {
            if (v[j] <= v[j + 1]) break;
            /* Swap this element back one step. */
            swap(v, j, j+1);
        }
    }
}


/*
 * Rearranges the elements of v into sorted order using
 * the merge sort algorithm.
 */
void mergeSort(Vector<int>& vec) {
    /* A list with 0 or 1 elements is already sorted by definition. */
    if (vec.size() <= 1) return;

    /* Split the list into two, equally sized halves */
    Vector<int> left, right;
    split(vec, left, right);

    /* Recursively sort the two halves. */
    mergeSort(left);
    mergeSort(right);

    /*
     * Empty out the original vector and re-fill it with merged result
     * of the two sorted halves.
     */
    vec = {};
    merge(vec, left, right);
}

/*
 * Given a sequence of n elements (vec), splits the sequence into two sublists
 * each of size n/2 and stores these sublists in the output parameters left and
 * right.
 */
void split(Vector<int>& vec, Vector<int>& left, Vector<int>& right){
    int n = vec.size();
    left = vec.subList(0, n / 2);
    right = vec.subList(n / 2);
}

/*
 * Merges the sorted sequences v1 and v2 into one large, sorted result.
 * Precondition: v1 and v2 are sorted, and vec is empty
 */
void merge(Vector<int>& vec, Vector<int>& v1, Vector<int>& v2) {
    int n1 = v1.size();
    int n2 = v2.size();
    int idx1 = 0;
    int idx2 = 0;
    /*
     * While both lists still have at least one element in them, compare the
     * smallest remaining value in each and take the smaller of the two to add
     * onto the result list.
     */
    while (idx1 < n1 && idx2 < n2) {
        if (v1[idx1] < v2[idx2]) {
            vec.add(v1[idx1]);
            idx1++;
        } else {
            vec.add(v2[idx2]);
            idx2++;
        }
    }

    /*
     * Add in any remaining/leftover elements that didn't get caught by the above
     * loop.
     */
    while (idx1 < n1) {
        vec.add(v1[idx1]);
        idx1++;
    }

    while (idx2 < n2) {
        vec.add(v2[idx2]);
        idx2++;
    }
}

void quickSort(Vector<int>& vec){
    /* A list with 0 or 1 elements is already sorted by definition. */
    if (vec.size() <= 1) return;

    /* Pick the pviot and partition the list into three components.
     * 1) elements less than the pivot
     * 2) elements equal to the pivot
     * 3) elements greater than the pivot
    */
    Vector<int> less, equal, greater;
    int pivot = vec[0];
    partition(vec, less, equal, greater, pivot);

    /* Recursively sort the two unsorted components. */
    quickSort(less);
    quickSort(greater);

    /*
     * Empty out the original vector and re-fill it with merged result
     * of the two sorted halves.
     */
    vec = {};
    concatenate(vec, less, equal, greater);
}

void partition(Vector<int>& vec, Vector<int>& less, Vector<int>& equal, Vector<int>& greater, int pivot) {
    for (int val: vec){
        if (val < pivot){
            less.add(val);
        } else if (val == pivot) {
            equal.add(val);
        } else {
            greater.add(val);
        }
    }
}

/* Assumes vec is empty when passed int othis function */
void concatenate(Vector<int>& vec, Vector<int>& less, Vector<int>& equal, Vector<int>& greater) {
    /*
     * Start with all sorted elements less than the pivot, then add in those equal,
     * followed by the sorted list of elements greater than pivot.
     */
    vec.addAll(less);
    vec.addAll(equal);
    vec.addAll(greater);
}

//PROVIDED_TEST("Simple test to test correctness of overall selection sort algorithm"){
//    Vector<int> vals;
//    Vector<int> soln;
//    fillRandomIntVector(vals, 10);
//    soln = vals;

//    /* Run selection sort algorithm. */
//    selectionSort(vals);
//    /* Use built-in sort funciton for solution */
//    soln.sort();

//    EXPECT_EQUAL(vals, soln);
//}

//PROVIDED_TEST("Timing Test for Selection Sort"){
//    int startSize = 5000;
//    for (int size = startSize; size <= 10 * startSize; size *= 2){
//        Vector<int> vals;
//        fillRandomIntVector(vals, size);
//        TIME_OPERATION(size, selectionSort(vals));
//    }
//}

//PROVIDED_TEST("Simple test to test correctness of overall insertion sort algorithm"){
//    Vector<int> vals;
//    Vector<int> soln;
//    fillRandomIntVector(vals, 10);
//    soln = vals;

//    /* Run insertion sort algorithm. */
//    insertionSort(vals);
//    /* Use built-in sort funciton for solution */
//    soln.sort();

//    EXPECT_EQUAL(vals, soln);
//}

//PROVIDED_TEST("Timing Test for Insertion Sort"){
//    int startSize = 5000;
//    for (int size = startSize; size <= 10 * startSize; size *= 2){
//        Vector<int> vals;
//        fillRandomIntVector(vals, size);
//        TIME_OPERATION(size, insertionSort(vals));
//    }
//}

PROVIDED_TEST("Simple test to test correctness of split helper function."){
    Vector<int> vals = {1, 2, 3, 4, 5, 6};
    Vector<int> left, right;
    split(vals, left, right);
    EXPECT_EQUAL(left, vals.subList(0, 3));
    EXPECT_EQUAL(right, vals.subList(3, 3));
}

PROVIDED_TEST("Simple test to test correctness of merge helper function."){
    Vector<int> v1 = {1, 5, 7};
    Vector<int> v2 = {4, 6, 10, 12, 14};
    Vector<int> result;
    merge(result, v1, v2);
    Vector<int> soln = {1, 4, 5, 6, 7, 10, 12, 14};
    EXPECT_EQUAL(result, soln);
}

PROVIDED_TEST("Simple test to test correctness of overall merge sort algorithm"){
    Vector<int> vals;
    Vector<int> soln;
    fillRandomIntVector(vals, 10);
    soln = vals;

    /* Run insertion sort algorithm. */
    mergeSort(vals);
    /* Use built-in sort funciton for solution */
    soln.sort();

    EXPECT_EQUAL(vals, soln);
}

PROVIDED_TEST("Timing Test for Merge Sort"){
    int startSize = 50000;
    for (int size = startSize; size <= 10 * startSize; size *= 2){
        Vector<int> vals;
        fillRandomIntVector(vals, size);
        TIME_OPERATION(size, mergeSort(vals));
    }
}

PROVIDED_TEST("Simple test to test correctness of partition helper function."){
    Vector<int> vals = {1, 2, 3, 4, 5, 6};
    Vector<int> less, equal, greater;
    int pivot = 3;
    partition(vals, less, equal, greater, pivot);
    EXPECT_EQUAL(less, vals.subList(0, 2));
    EXPECT_EQUAL(equal, vals.subList(2, 1));
    EXPECT_EQUAL(greater, vals.subList(3, 3));
}

PROVIDED_TEST("Simple test to test correctness of concatenate helper function."){
    Vector<int> less = {1, 2};
    Vector<int> equal = {3};
    Vector<int> greater = {4, 5, 6};
    Vector<int> result;
    concatenate(result, less, equal, greater);
    Vector<int> soln = less + equal + greater;
    EXPECT_EQUAL(result, soln);
}

PROVIDED_TEST("Simple test to test correctness of overall quicksort algorithm"){
    Vector<int> vals;
    Vector<int> soln;
    fillRandomIntVector(vals, 10);
    soln = vals;

    /* Run insertion sort algorithm. */
    quickSort(vals);
    /* Use built-in sort funciton for solution */
    soln.sort();

    EXPECT_EQUAL(vals, soln);
}

PROVIDED_TEST("Timing Test for Quicksort"){
    int startSize = 50000;
    for (int size = startSize; size <= 10 * startSize; size *= 2){
        Vector<int> vals;
        fillRandomIntVector(vals, size);
        TIME_OPERATION(size, quickSort(vals));
    }
}
