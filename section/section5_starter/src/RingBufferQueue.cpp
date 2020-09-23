#include "RingBufferQueue.h"
#include "testing/SimpleTest.h"

using namespace std;

const int kDefaultCapacity = 5;
/**
 * TODO: write a method comment
 */
RBQueue::RBQueue() {
   elements = new int[kDefaultCapacity];
   head = 0;
   usedItem = 0;
   capacity = kDefaultCapacity;
}

/**
 * TODO: write a method comment
 */
RBQueue::~RBQueue() {
    delete[] elements;

}

/**
 * TODO: write a method comment
 */
void RBQueue::enqueue(int elem) {
    if(isFull()){
        // expand and set head to 0
        int *newElements = new int[capacity * 2];
        for(int bias = 0; bias < capacity; bias++){
            newElements[bias] = elements[(head + bias) % capacity];
        }
        delete[] elements;
        elements = newElements;
        capacity *= 2;
        head = 0;
    }
   int tail = (head + usedItem) % capacity;
   elements[tail] = elem;
   usedItem++;
}

/**
 * TODO: write a method comment
 */
int RBQueue::dequeue() {
    if(isEmpty()){
        throw "Can't dequeue from any empty queue!";
    }
    int oldHead = head;
    head = (head + 1) % capacity;
    usedItem--;
    return elements[oldHead];
}

/*重用了peek()的代码
int RingBufferQueue::dequeue() {
    int front = peek();
    _head = (_head + 1) % _capacity;
    _size--;
    return front;
}
*/
/**
 * TODO: write a method comment
 */
int RBQueue::peek() const{
    if(isEmpty()){
        throw "Can't peek from any empty queue!";
    }
    return elements[head];
}

/**
 * TODO: write a method comment
 */
bool RBQueue::isEmpty() const{
    return usedItem == 0;
}

/**
 * TODO: write a method comment
 */
bool RBQueue::isFull() const{
    return usedItem == capacity;
}

/**
 * TODO: write a method comment
 */
int RBQueue::size() const{
    return usedItem;
}

/**
 * TODO: write a method comment
 */
ostream& operator<<(ostream& out, const RBQueue& queue) {
    out << "{";
    if(!queue.isEmpty()){
        out << queue.elements[queue.head];
        for(int i = 1; i < queue.usedItem; i++){
            int index = (queue.head + i) % queue.capacity;
            out << ", " << queue.elements[index];
        }
    }
    out << "}";
    return out;
}

/* Provided Tests Below This Line */

PROVIDED_TEST("Simple Example From Handout, No resizing"){
    RBQueue q;
    q.enqueue(1);
    q.enqueue(2);
    q.enqueue(3);
    EXPECT_EQUAL(q.peek(), 1);
    q.enqueue(4);
    q.enqueue(5);
    EXPECT(q.isFull());
    EXPECT_EQUAL(q.dequeue(), 1);
    EXPECT_EQUAL(q.dequeue(), 2);
    EXPECT_EQUAL(q.peek(), 3);
    q.enqueue(6);
    EXPECT_EQUAL(q.dequeue(), 3);
    EXPECT_EQUAL(q.dequeue(), 4);
    EXPECT_EQUAL(q.dequeue(), 5);
    EXPECT_EQUAL(q.dequeue(), 6);
    EXPECT(q.isEmpty());
}

PROVIDED_TEST("Example from Handout, with Resizing"){
    RBQueue q;
    for (int i = 10; i <= 50; i+= 10){
        q.enqueue(i);
    }
    EXPECT(q.isFull());
    // Adding the 6th element should trigger a resizing
    q.enqueue(60);
    EXPECT(!q.isFull());
    for (int i = 10; i <= 60; i += 10){
        EXPECT_EQUAL(i, q.peek());
        EXPECT_EQUAL(i, q.dequeue());
    }
    EXPECT(q.isEmpty());
}
