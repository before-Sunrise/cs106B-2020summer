/* 
 * TODO: remove and replace this file header comment
 * You will edit and turn in this file.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include <iostream>    // for cout, endl
#include <string>      // for string class
#include "queue.h"
#include "testing/SimpleTest.h"
using namespace std;

/* 
 * TODO: Replace this comment with a descriptive function
 * header comment.
 */

bool checkOrder(Queue<int> a){
    while(a.size() > 1){
        int first = a.dequeue();
        int second = a.dequeue();
        if(first > second){
            return false;
        }
    }
    // 此时a.size == 1/0，必有序
    return true;
}

Queue<int> merge(Queue<int> a, Queue<int> b) {
    if(!checkOrder(a) || !checkOrder(b)){
        error("queue a or b is not ordered");
    }
    Queue<int> result;
    int left, right;
    while(!a.isEmpty() && !b.isEmpty()){
        left = a.peek();
        right = b.peek();
        if(left < right){
            result.add(left);
            a.dequeue();
        }else{
            result.add(right);
            b.dequeue();
        }
    }
    while(!a.isEmpty()){
        result.enqueue(a.dequeue());
    }
    while(!b.isEmpty()){
        result.enqueue(b.dequeue());
    }
    return result;
}

/* 
 * This function assumes correct functionality of the previously
 * defined merge function and makes use of this function to
 * iteratively merge a collection of sequences, starting off with
 * an empty sequence and progressively merging in one sequence at
 * a time. This function is provided for you fully implemented – 
 * it does not need to modified at all.
 */
Queue<int> multiMerge(Vector<Queue<int>>& all) {
    Queue<int> result;
    for (Queue<int>& q : all) {
        result = merge(q, result);
    }
    return result;
}

/* 
 * TODO: Replace this comment with a descriptive function
 * header comment.
 */
Queue<int> recMultiMerge(Vector<Queue<int>>& all) {
    Queue<int> result;
    if(all.size() == 1){
        return all.front();
    }else if(all.size() == 2){
        result = merge(all.front(), all.back());
        return result;
    }else{
        int mid = all.size() / 2;
        Vector<Queue<int>> first, second;
        Queue<int> front, end;
        for(int i = 0; i < mid; i++){
            first.add(all[i]);
        }
        for(int i = mid; i < all.size(); i++){
            second.add(all[i]);
        }
        front = recMultiMerge(first);
        end = recMultiMerge(second);
        return merge(front, end);
    }
}


/* * * * * * Test Cases * * * * * */

Queue<int> createSequence(int size);
void distribute(Queue<int> input, Vector<Queue<int>>& all);

PROVIDED_TEST("Test binary merge, two short sequences") {
    Queue<int> a = {2, 4, 5};
    Queue<int> b = {1, 3, 3};
    Queue<int> expected = {1, 2, 3, 3, 4, 5};
    EXPECT_EQUAL(merge(a, b), expected);
    EXPECT_EQUAL(merge(b, a), expected);
}

PROVIDED_TEST("Test multiMerge, small collection of short sequences") {
    Vector<Queue<int>> all = {{3, 6, 9, 9, 100},
                             {1, 5, 9, 9, 12},
                             {5},
                             {},
                             {-5, -5},
                             {3402}
                            };
    Queue<int> expected = {-5, -5, 1, 3, 5, 5, 6, 9, 9, 9, 9, 12, 100, 3402};
    EXPECT_EQUAL(multiMerge(all), expected);
}

PROVIDED_TEST("Test recMultiMerge by compare to multiMerge") {
    int n = 1000;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(n);
    distribute(input, all);
    EXPECT_EQUAL(multiMerge(all), recMultiMerge(all));
}

PROVIDED_TEST("Time binary merge operation") {
    int n = 1500000;
    Queue<int> a = createSequence(n);
    Queue<int> b = createSequence(n);
    TIME_OPERATION(a.size(), merge(a, b));
}

PROVIDED_TEST("Time multiMerge operation") {
    int n = 11000;
    int k = n/10;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(k);
    distribute(input, all);
    TIME_OPERATION(input.size(), multiMerge(all));
}
PROVIDED_TEST("Time recMultiMerge operation") {
    int n = 90000;
    int k = n/10;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(k);
    distribute(input, all);
    TIME_OPERATION(input.size(), recMultiMerge(all));
}


/* Test helper to fill queue with sorted sequence */
Queue<int> createSequence(int size) {
    Queue<int> q;
    for (int i = 0; i < size; i++) {
        q.enqueue(i);
    }
    return q;
}

/* Test helper to distribute elements of sorted sequence across k sequences,
   k is size of Vector */
// 将input的各个元素随机分到all的各个queue中，并且只要input升序，各个queue也一定升序
void distribute(Queue<int> input, Vector<Queue<int>>& all) {
    while (!input.isEmpty()) {
        all[randomInteger(0, all.size()-1)].add(input.dequeue());
    }
}

STUDENT_TEST("Test binary merge with empty queue") {
    Queue<int> a = {};
    Queue<int> b = {1, 3, 6, 9};
    Queue<int> expected = {1, 3, 6, 9};
    EXPECT_EQUAL(merge(a, b), expected);
    EXPECT_EQUAL(merge(b, a), expected);
}


STUDENT_TEST("Test binary merge with unsorted queue") {
    Queue<int> a = {7, 2, 5};
    Queue<int> b = {1, 3, 6, 9};
    EXPECT_ERROR(merge(a, b));
}

STUDENT_TEST("Test binary merge's Big O") {
    for(int i = 1; i < 5; i++){
        Queue<int> a = createSequence(1500000 * i);
        Queue<int> b = createSequence(1500000 * i);
        TIME_OPERATION(a.size(), merge(a, b));
    }
}

STUDENT_TEST("Test multiMerge with 0 queue"){
    Vector<Queue<int>> all{ };
    Queue<int> empty{};
    EXPECT_EQUAL(multiMerge(all), empty);
}

STUDENT_TEST("Test multiMerge with multiple empty queues"){
    Vector<Queue<int>> all{{}, {}, {}, {} };
    Queue<int> empty{};
    EXPECT_EQUAL(multiMerge(all), empty);
}

STUDENT_TEST("Time multiMerge with n changed and k fixed") {
    int n = 11000;
    int k = n/10;
    for(int i = 1; i < 6; i++){
        Queue<int> input = createSequence(n * i);
        Vector<Queue<int>> all(k);
        distribute(input, all);
        TIME_OPERATION(input.size(), multiMerge(all));
    }
}

STUDENT_TEST("Time multiMerge with k changed and n fixed") {
    int n = 11000;
    int k = n/100;
    for(int i = 1; i < 6; i++){
        Queue<int> input = createSequence(n);
        Vector<Queue<int>> all(k * i);
        distribute(input, all);
        TIME_OPERATION(k * i, multiMerge(all));
    }
}

STUDENT_TEST("Time recMultiMerge with n changed and k fixed") {
    int n = 11000;
    int k = n/10;
    for(int i = 1; i < 6; i++){
        Queue<int> input = createSequence(n * i);
        Vector<Queue<int>> all(k);
        distribute(input, all);
        TIME_OPERATION(input.size(), recMultiMerge(all));
    }
}

STUDENT_TEST("Time recMultiMerge with k changed and n fixed") {
    int n = 11000;
    int k = n/100;
    for(int i = 1; i < 6; i++){
        Queue<int> input = createSequence(n);
        Vector<Queue<int>> all(k * i);
        distribute(input, all);
        TIME_OPERATION(k * i, recMultiMerge(all));
    }
}
