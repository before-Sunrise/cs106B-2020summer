#include "vector.h"
#include "testing/SimpleTest.h"
#include "random.h"

using namespace std;

int binarySearch(Vector<int>& v, int targetVal);
int binarySearchHelper(Vector<int>& v, int targetVal, int lowerBound, int upperBound);
int linearSearch(Vector<int>& v, int targetVal);


int binarySearch(Vector<int>& v, int targetVal) {
    return binarySearchHelper(v, targetVal, 0, v.size() - 1);
}

int binarySearchHelper(Vector<int>& v, int targetVal, int startIndex, int endIndex) {
    if (startIndex > endIndex) {
        return -1;
    }

    int middleIndex = (startIndex + endIndex) / 2;
    int currentVal = v[middleIndex];
    if (targetVal == currentVal) {
        return middleIndex;
    } else if (targetVal < currentVal) {
        return binarySearchHelper(v, targetVal, startIndex, middleIndex - 1);
    } else {
        return binarySearchHelper(v, targetVal, middleIndex + 1, endIndex);
    }
}

int linearSearch(Vector<int>& v, int targetVal) {
    for (int i = 0; i < v.size(); i++) {
        if (v[i] == targetVal) {
            return i;
        }
    }
    return -1;
}

PROVIDED_TEST("Test binarySearch on example input") {
    Vector<int> testVals = {-5, -1, 0, 3, 17, 106, 1047, 1738};
    EXPECT_EQUAL(binarySearch(testVals, -5), 0);
    EXPECT_EQUAL(binarySearch(testVals, -1), 1);
    EXPECT_EQUAL(binarySearch(testVals, 0), 2);
    EXPECT_EQUAL(binarySearch(testVals, 3), 3);
    EXPECT_EQUAL(binarySearch(testVals, 17), 4);
    EXPECT_EQUAL(binarySearch(testVals, 106), 5);
    EXPECT_EQUAL(binarySearch(testVals, 1047), 6);
    EXPECT_EQUAL(binarySearch(testVals, 1738), 7);
    EXPECT_EQUAL(binarySearch(testVals, 107), -1);
    EXPECT_EQUAL(binarySearch(testVals, -100), -1);
    EXPECT_EQUAL(binarySearch(testVals, 1), -1);
    EXPECT_EQUAL(binarySearch(testVals, 80000), -1);
}

Vector<int> createSequence(int size) {
    Vector<int> v;
    for (int i = 0; i < size; i++){
        v.add(i);
    }
    return v;
}

void findRandomElementsBinary(Vector<int>& v, int maxVal, int numToFind) {
    for (int i = 0; i < numToFind; i++) {
        int targetVal = randomInteger(0, maxVal);
        binarySearch(v, targetVal);
    }
}

void findRandomElementsLinear(Vector<int>& v, int maxVal, int numToFind){
    for (int i = 0; i < numToFind; i++){
        int targetVal = randomInteger(0, maxVal);
        linearSearch(v, targetVal);
    }
}

PROVIDED_TEST("Time Trials Experiments for binary search"){
    int N = 1000000;
    int numToFind = 100000;
    Vector<int> testVals = createSequence(N);

    TIME_OPERATION(N, findRandomElementsBinary(testVals, N, numToFind));

    N = 2000000;
    testVals = createSequence(N);
    TIME_OPERATION(N, findRandomElementsBinary(testVals, N, numToFind));

    N = 4000000;
    testVals = createSequence(N);
    TIME_OPERATION(N, findRandomElementsBinary(testVals, N, numToFind));

    N = 8000000;
    testVals = createSequence(N);
    TIME_OPERATION(N, findRandomElementsBinary(testVals, N, numToFind));

    N = 16000000;
    testVals = createSequence(N);
    TIME_OPERATION(N, findRandomElementsBinary(testVals, N, numToFind));

    N = 32000000;
    testVals = createSequence(N);
    TIME_OPERATION(N, findRandomElementsBinary(testVals, N, numToFind));

    N = 64000000;
    testVals = createSequence(N);
    TIME_OPERATION(N, findRandomElementsBinary(testVals, N, numToFind));
}

PROVIDED_TEST("Time Trials Experiments for linear search"){
    int N = 10000;
    int numToFind = 1000;
    Vector<int> testVals = createSequence(N);

    TIME_OPERATION(N, findRandomElementsLinear(testVals, N, numToFind));

    N = 20000;
    testVals = createSequence(N);
    TIME_OPERATION(N, findRandomElementsLinear(testVals, N, numToFind));

    N = 40000;
    testVals = createSequence(N);
    TIME_OPERATION(N, findRandomElementsLinear(testVals, N, numToFind));

    N = 80000;
    testVals = createSequence(N);
    TIME_OPERATION(N, findRandomElementsLinear(testVals, N, numToFind));

    N = 160000;
    testVals = createSequence(N);
    TIME_OPERATION(N, findRandomElementsLinear(testVals, N, numToFind));

    N = 320000;
    testVals = createSequence(N);
    TIME_OPERATION(N, findRandomElementsLinear(testVals, N, numToFind));

    N = 640000;
    testVals = createSequence(N);
    TIME_OPERATION(N, findRandomElementsLinear(testVals, N, numToFind));
}
