#include "pqheap.h"
#include "error.h"
#include "random.h"
#include "strlib.h"
#include "datapoint.h"
#include "testing/SimpleTest.h"
using namespace std;

const int INITIAL_CAPACITY = 10;

/*
 * TODO: Replace this comment with a descriptive function
 * header comment about your implementation of the function.
 */
PQHeap::PQHeap() {
    minHeap = new DataPoint[INITIAL_CAPACITY];
    usedItem = 0;
    allocatedItem = INITIAL_CAPACITY;
}

/*
 * TODO: Replace this comment with a descriptive function
 * header comment about your implementation of the function.
 */
PQHeap::~PQHeap() {
    delete[] minHeap;
}

/*
 * TODO: Replace this comment with a descriptive function
 * header comment about your implementation of the function.
 */
void PQHeap::enqueue(DataPoint elem) {
    if(usedItem == allocatedItem){
        expand();
    }
    minHeap[usedItem] = elem;
    usedItem ++;
    bubbleUp();
}

/*
 * TODO: Replace this comment with a descriptive function
 * header comment about your implementation of the function.
 */
DataPoint PQHeap::peek() const {
    if(isEmpty()){
        error("can't peek an empty priority queue");
    }
    return minHeap[0];
}

/*
 * TODO: Replace this comment with a descriptive function
 * header comment about your implementation of the function.
 */
DataPoint PQHeap::dequeue() {
    if(isEmpty()){
        error("can't dequeue from an empty queue");
    }
    DataPoint head = minHeap[0];
    minHeap[0] = minHeap[usedItem - 1];
    usedItem--;
    bubbleDown();
    return  head;
}

/*
 * TODO: Replace this comment with a descriptive function
 * header comment about your implementation of the function.
 */
bool PQHeap::isEmpty() const {
    return usedItem == 0;
}

/*
 * TODO: Replace this comment with a descriptive function
 * header comment about your implementation of the function.
 */
int PQHeap::size() const {
    return usedItem;
}

/*
 * TODO: Replace this comment with a descriptive function
 * header comment about your implementation of the function.
 */
void PQHeap::clear() {
    usedItem = 0;
}

/*
 * TODO: Replace this comment with a descriptive function
 * header comment about your implementation of the function.
 */
void PQHeap::printDebugInfo() {
    /* TODO: Fill in this function. */
}

/*
 * We strongly recommend implementing this helper function, which
 * should traverse the heap array and ensure that the heap property
 * holds for all elements in the array. If elements are found that
 * violate the heap property, an error should be thrown.
 */
void PQHeap::validateInternalState(){
    for(int index = 1; index < usedItem; index++){
        int parentIndex = getParentIndex(index);
        if(minHeap[parentIndex].priority >  minHeap[index].priority){
            error("parent is bigger than children");
        }
    }
}

/* 
 * We strongly recommend implementing this helper function, which 
 * should calculate the index of the parent of the element with the
 * provided index.
 */
int PQHeap::getParentIndex(int curIndex){
    return (curIndex - 1) / 2;
}

/* 
 * We strongly recommend implementing this helper function, which 
 * should calculate the index of the left child of the element with the
 * provided index.
 */
int PQHeap::getLeftChildIndex(int curIndex){
    return 2 * curIndex + 1;
}

/* 
 * We strongly recommend implementing this helper function, which 
 * should calculate the index of the right child of the element with the
 * provided index.
 */
int PQHeap::getRightChildIndex(int curIndex){
    return 2 * curIndex + 2;
}

void PQHeap::expand(){
    DataPoint* newHeap = new DataPoint[allocatedItem * 2];
    for(int i = 0; i < allocatedItem; i++){
        newHeap[i] = minHeap[i];
    }
    allocatedItem *= 2;
    delete[] minHeap;
    minHeap = newHeap;
}

void PQHeap::bubbleUp(){
    int index = usedItem - 1; // 指向需要bubble up的元素
    while(index != 0){
        int parentIndex = getParentIndex(index);
        if(minHeap[parentIndex].priority < minHeap[index].priority){
            break;
        }
        DataPoint temp = minHeap[index];
        minHeap[index] = minHeap[parentIndex];
        minHeap[parentIndex] = temp;
        index = parentIndex;
    }
}

void PQHeap::bubbleDown(){
    int index = 0;
    int leftChildren = getLeftChildIndex(index);
    int rightChildren = getRightChildIndex(index);
    while(leftChildren < usedItem || rightChildren < usedItem){ //当index对应的结点为父节点时
        if(rightChildren < usedItem){ //左右孩子都存在
            int lPriority = minHeap[leftChildren].priority;
            int rPriority = minHeap[rightChildren].priority;
            if(lPriority < rPriority){
                if(lPriority < minHeap[index].priority){ //左孩子小于右孩子且小于父亲，则将父亲和左孩子交换
                    DataPoint point = minHeap[index];
                    minHeap[index] = minHeap[leftChildren];
                    minHeap[leftChildren] = point;
                    index = leftChildren; // 再将父亲置为左孩子
                }else{ //左孩子小于右孩子，大于等于父亲，则bubble down完成
                    break;
                }
            }else{
                if(rPriority < minHeap[index].priority){ //右孩子小于左孩子且小于父亲，则将父亲和右孩子交换
                    DataPoint point = minHeap[index];
                    minHeap[index] = minHeap[rightChildren];
                    minHeap[rightChildren] = point;
                    index = rightChildren; // 再将父亲置为右孩子
                }else{ //左孩子小于右孩子，大于等于父亲，则bubble down完成
                    break;
                }
            }
        }else{ //只存在左孩子
            int lPriority = minHeap[leftChildren].priority;
            if(lPriority < minHeap[index].priority){ //左孩子小于父亲，则将父亲和左孩子交换
                DataPoint point = minHeap[index];
                minHeap[index] = minHeap[leftChildren];
                minHeap[leftChildren] = point;
                index = leftChildren; // 再将父亲置为左孩子
            }else{
                break;
            }
        }
        leftChildren = getLeftChildIndex(index);
        rightChildren = getRightChildIndex(index);

    }
}

/* * * * * * Test Cases Below This Point * * * * * */

/* TODO: Add your own custom tests here! */




/* * * * * Provided Tests Below This Point * * * * */

PROVIDED_TEST("Provided Test: Newly-created heap is empty.") {
    PQHeap pq;

    EXPECT(pq.isEmpty());
    EXPECT(pq.size() == 0);
}

PROVIDED_TEST("Provided Test: Enqueue / dequeue single element (two cycles)") {
    PQHeap pq;
    DataPoint point = { "Programming Abstractions", 106 };
    pq.enqueue(point);
    EXPECT_EQUAL(pq.size(), 1);
    EXPECT_EQUAL(pq.isEmpty(), false);

    EXPECT_EQUAL(pq.peek(), point);
    EXPECT_EQUAL(pq.dequeue(), point);
    EXPECT_EQUAL(pq.size(), 0);
    EXPECT_EQUAL(pq.isEmpty(), true);

    pq.enqueue(point);
    EXPECT_EQUAL(pq.size(), 1);
    EXPECT_EQUAL(pq.isEmpty(), false);

    EXPECT_EQUAL(pq.peek(), point);
    EXPECT_EQUAL(pq.dequeue(), point);
    EXPECT_EQUAL(pq.size(), 0);
    EXPECT_EQUAL(pq.isEmpty(), true);
}

PROVIDED_TEST("Provided Test: Test clear operation works with single element."){
    PQHeap pq;
    DataPoint point = { "Programming Abstractions", 106 };

    pq.enqueue(point);
    pq.clear();
    EXPECT_EQUAL(pq.size(), 0);
    EXPECT_EQUAL(pq.isEmpty(), true);
}

PROVIDED_TEST("Provided Test: Dequeue / peek on empty priority queue throws error") {
    PQHeap pq;

    EXPECT(pq.isEmpty());
    EXPECT_ERROR(pq.dequeue());
    EXPECT_ERROR(pq.peek());
}

PROVIDED_TEST("Provided Test: Dequeue / peek on recently-cleared priority queue throws error") {
    PQHeap pq;
    DataPoint point = { "Programming Abstractions", 106 };

    pq.enqueue(point);
    pq.clear();
    EXPECT(pq.isEmpty());
    EXPECT_ERROR(pq.dequeue());
    EXPECT_ERROR(pq.peek());
}

PROVIDED_TEST("Provided Test: Enqueue elements in sorted order.") {
    PQHeap pq;
    for (int i = 0; i < 25; i++) {
        pq.enqueue({ "elem" + integerToString(i), i });
    }

    EXPECT_EQUAL(pq.size(), 25);
    for (int i = 0; i < 25; i++) {
        DataPoint removed = pq.dequeue();
        DataPoint expected = {
            "elem" + integerToString(i), i
        };
        EXPECT_EQUAL(removed, expected);
    }
    EXPECT_EQUAL(pq.size(), 0);
    EXPECT_EQUAL(pq.isEmpty(), true);
}

PROVIDED_TEST("Provided Test: Enqueue elements in reverse-sorted order.") {
    PQHeap pq;
    for (int i = 25; i >= 0; i--) {
        pq.enqueue({ "elem" + integerToString(i), i });
    }

    EXPECT_EQUAL(pq.size(), 26);
    for (int i = 0; i <= 25; i++) {
        DataPoint removed = pq.dequeue();
        DataPoint expected = {
            "elem" + integerToString(i), i
        };
        EXPECT_EQUAL(removed, expected);
    }
    EXPECT_EQUAL(pq.size(), 0);
    EXPECT_EQUAL(pq.isEmpty(), true);
}

PROVIDED_TEST("Provided Test: Insert ascending and descending sequences.") {
    PQHeap pq;
    for (int i = 0; i < 20; i++) {
        pq.enqueue({ "a" + integerToString(i), 2 * i });
    }
    for (int i = 19; i >= 0; i--) {
        pq.enqueue({ "b" + integerToString(i), 2 * i + 1 });
    }

    EXPECT_EQUAL(pq.size(), 40);
    for (int i = 0; i < 40; i++) {
        DataPoint removed = pq.dequeue();
        EXPECT_EQUAL(removed.priority, i);
    }
    EXPECT_EQUAL(pq.size(), 0);
    EXPECT_EQUAL(pq.isEmpty(), true);
}

PROVIDED_TEST("Provided Test: Insert random sequence of elements.") {
    Vector<DataPoint> sequence = {
        { "A", 0 },
        { "D", 3 },
        { "F", 5 },
        { "G", 6 },
        { "C", 2 },
        { "H", 7 },
        { "I", 8 },
        { "B", 1 },
        { "E", 4 },
        { "J", 9 },
    };

    PQHeap pq;
    for (DataPoint elem: sequence) {
        pq.enqueue(elem);
    }

    EXPECT_EQUAL(pq.size(), sequence.size());

    for (int i = 0; i < 10; i++) {
        DataPoint removed = pq.dequeue();
        DataPoint expected = {
            charToString('A' + i), i
        };
        EXPECT_EQUAL(removed, expected);
    }
    EXPECT_EQUAL(pq.size(), 0);
    EXPECT_EQUAL(pq.isEmpty(), true);
}


PROVIDED_TEST("Provided Test: Insert duplicate elements.") {
    PQHeap pq;
    for (int i = 0; i < 20; i++) {
        pq.enqueue({ "a" + integerToString(i), i });
    }
    for (int i = 19; i >= 0; i--) {
        pq.enqueue({ "b" + integerToString(i), i });
    }

    EXPECT_EQUAL(pq.size(), 40);
    for (int i = 0; i < 20; i++) {
        DataPoint one = pq.dequeue();
        DataPoint two = pq.dequeue();

        EXPECT_EQUAL(one.priority, i);
        EXPECT_EQUAL(two.priority, i);
    }
    EXPECT_EQUAL(pq.size(), 0);
    EXPECT_EQUAL(pq.isEmpty(), true);
}

PROVIDED_TEST("Provided Test: Handles data points with empty string name.") {
    PQHeap pq;
    for (int i = 0; i < 10; i++) {
        pq.enqueue({ "" , i });
    }
    EXPECT_EQUAL(pq.size(), 10);
}

PROVIDED_TEST("Test enqueue/dequeue of longer random sequence") {
    PQHeap pq;

    for (int i = 0; i < 100; i++) {
        int randomValue = randomInteger(0, 100);
        DataPoint pt = {"elem" + integerToString(randomValue), randomValue};
        pq.enqueue(pt);
    }
    EXPECT_EQUAL(pq.size(), 100);
    DataPoint last = {"", -1};
    for (int i = 0; i < 100; i++) {
        DataPoint cur = pq.dequeue();
        EXPECT(cur.priority >= 0 && cur.priority <= 100 && cur.priority >= last.priority);
        last = cur;
    }
    EXPECT_EQUAL(pq.size(), 0);
}


PROVIDED_TEST("Provided Test: Handles data points with negative weights.") {
    PQHeap pq;
    for (int i = -10; i < 10; i++) {
        pq.enqueue({ "" , i });
    }
    EXPECT_EQUAL(pq.size(), 20);
    for (int i = -10; i < 10; i++) {
        DataPoint removed = pq.dequeue();
        EXPECT_EQUAL(removed.priority, i);
    }
}

PROVIDED_TEST("Provided Test: Interleave enqueues and dequeues.") {
    PQHeap pq;
    int n = 100;
    for (int i = n / 2; i < n; i++) {
        pq.enqueue({"", i});
    }
    EXPECT_EQUAL(pq.size(), 50);
    for (int i = n / 2; i < n; i++) {
        EXPECT_EQUAL(pq.dequeue().priority, i);
    }
    EXPECT_EQUAL(pq.size(), 0);

    for (int i = 0; i < n / 2; i++) {
        pq.enqueue({"", i});
    }
    EXPECT_EQUAL(pq.size(), 50);
    for (int i = 0; i < n / 2; i++) {
        EXPECT_EQUAL(pq.dequeue().priority, i);
    }
    EXPECT_EQUAL(pq.size(), 0);
}

static void fillAndEmpty(int n) {
    PQHeap pq;
    DataPoint max = {"max", 106106106};
    DataPoint min = {"min", -106106106};

    pq.enqueue(min);
    pq.enqueue(max);
    for (int i = 0; i < n; i++) {
        int randomPriority = randomInteger(-10000, 10000);
        pq.enqueue({ "", randomPriority });
    }
    EXPECT_EQUAL(pq.size(), n + 2);

    EXPECT_EQUAL(pq.dequeue(), min);
    for (int i = 0; i < n; i++) {
        pq.dequeue();
    }
    EXPECT_EQUAL(pq.dequeue(), max);
    EXPECT_EQUAL(pq.size(), 0);
}

PROVIDED_TEST("Provided Test: Stress Test. Time the amount of time it takes to cycle many elements in and out. Should take at most about 5-10 seconds.") {
    TIME_OPERATION(20000, fillAndEmpty(20000));
}
