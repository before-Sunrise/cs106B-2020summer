#include "testing/SimpleTest.h"
#include "stack.h"
#include "queue.h"
#include "map.h"
using namespace std;

/* This function correctly reverses the elements of a queue.
 * just teach you how to ues debug on ADT
 */
void reverse(Queue<int>& q) {
    Stack<int> s;
    while (!q.isEmpty()) {
        int val = q.dequeue();
        s.push(val);
    }
    while (!s.isEmpty()) {
        int val = s.pop();
        q.enqueue(val);
    }
}

/* This function is intended to modify a queue of characters to duplicate
 * any negative numbers.
 *
 * WARNING: the given code is buggy. See exercise writeup for more
 * information on how to test and diagnose.
 */

/* original buggy code
void duplicateNegatives(Queue<int>& q) {
    for (int i = 0; i < q.size(); i++) {
        int cur = q.dequeue();
        q.enqueue(cur);
        if (cur < 0) {
            q.enqueue(cur);   // double up on negative numbers
        }
    }
}
*/

// my solution
void duplicateNegatives(Queue<int>& q) {
    int count = q.size();
    while(count > 0){
        int val = q.dequeue();
        q.enqueue(val);
        if(val < 0){
            q.enqueue(val);
        }
        count--;
    }
}

// This function is intended to remove key/value pairs from
// map where key == value
// WARNING: the given code is buggy. See exercise writeup for more
// information on how to test and diagnose.
// the goal is to learn how to recognize when a test case raises an error.
void removeMatchPairs(Map<string, string>& map) {
    for (string key: map) {
        if (map[key] == key) {
            map.remove(key);
        }
    }
}

/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("reverse queue") {
    Queue<int> a = {1, 2, 3, 4, 5};
    Queue<int> b = {5, 4, 3, 2, 1};

    reverse(a);
    EXPECT_EQUAL(a, b);
}

PROVIDED_TEST("duplicateNegatives, input has no negatives") {
    Queue<int> a = {2, 10};
    Queue<int> b = a;

    duplicateNegatives(a);
    EXPECT_EQUAL(a, b);
}

PROVIDED_TEST("duplicateNegatives, input has single negative") {
    Queue<int> a = {-6, 7};
    Queue<int> b = {-6, -6, 7};

    duplicateNegatives(a);
    EXPECT_EQUAL(a, b);
}

PROVIDED_TEST("duplicateNegatives, input has some negatives") {
    Queue<int> a = {-3, 4, -5, 10};
    Queue<int> b = {-3, -3, 4, -5, -5, 10};

    duplicateNegatives(a);
    EXPECT_EQUAL(a, b);
}

PROVIDED_TEST("removeMatchPair, no change") {
    Map<string, string> a = {{"Thomas", "Tom"}, {"Margaret", "Meg"}};
    Map<string, string> b = a;

    removeMatchPairs(a);
    EXPECT_EQUAL(a, b);
}

PROVIDED_TEST("removeMatchPair, remove one") {
    Map<string, string> a = {{"Thomas", "Tom"}, {"Jan", "Jan"}, {"Margaret", "Meg"}};
    Map<string, string> b = {{"Thomas", "Tom"},  {"Margaret", "Meg"}};

    removeMatchPairs(a);
    EXPECT_EQUAL(a, b);
}

STUDENT_TEST("duplicateNegatives, input end with negatives"){
    Queue<int> a = {3, 3, -5};
    Queue<int> b = {3, 3, -5, -5};
    duplicateNegatives(a);
    EXPECT_EQUAL(a, b);
}
STUDENT_TEST("duplicateNegatives, input start with negatives"){
    Queue<int> a = {-3, 3, 5};
    Queue<int> b = {-3, -3, 3, 5};
    duplicateNegatives(a);
    EXPECT_EQUAL(a, b);
}
STUDENT_TEST("duplicateNegatives, input with even negatives"){
    Queue<int> a = {-3, 4, -5, 6};
    Queue<int> b = {-3, -3, 4, -5, -5, 6};
    duplicateNegatives(a);
    EXPECT_EQUAL(a, b);
}

STUDENT_TEST("duplicateNegatives, input with all negatives"){
    Queue<int> a = {-3, -4, -5};
    Queue<int> b = {-3, -3, -4, -4, -5, -5};
    duplicateNegatives(a);
    EXPECT_EQUAL(a, b);
}



