#pragma once
#include "testing/MemoryUtils.h"
#include "datapoint.h"

/**
 * Priority queue of DataPoints implemented using a sorted array.
 */
class PQSortedArray {
public:
    /**
     * Creates a new, empty priority queue.
     */
    PQSortedArray();

    /**
     * Cleans up all memory allocated by this priority queue.
     */
    ~PQSortedArray();

    /**
     * Adds a new element into the queue. This operation runs in time O(N),
     * where n is the number of elements in the queue.
     *
     * @param element The element to add.
     */
    void enqueue(DataPoint element);

    /**
     * Removes and returns the element that is frontmost in the priority queue.
     * The frontmost element is the one with lowest priority value.
     *
     * If the priority queue is empty, this function calls error().
     *
     * This operation must run in time O(1).
     *
     * @return The frontmost element, which is removed from queue.
     */
    DataPoint dequeue();

    /**
     * Returns, but does not remove, the element that is frontmost.
     *
     * If the priority queue is empty, this function calls error().
     *
     * This operation must run in time O(1).
     *
     * @return frontmost element
     */
    DataPoint peek() const;

    /**
     * Returns whether the priority queue is empty.
     *
     * This operation must run in time O(1).
     *
     * @return Whether the priority queue is empty.
     */
    bool isEmpty() const;

    /**
     * Returns the number of elements in this priority queue.
     *
     * This operation must run in time O(1).
     *
     * @return The number of elements in the priority queue.
     */
    int size() const;

    /**
     * Removes all elements from the priority queue.
     *
     * This operation must run in time O(1).
     */
    void clear();

    /**
     * This function exists purely for testing purposes. You can have it do
     * whatever you'd like. We will not invoke it when grading.
     *
     * The provided implementation prints out the contents of the array
     * for easy viewing purposes.
     */
    void printDebugInfo();

private:
    DataPoint* elements; // array of elements
    int allocatedCapacity;  // number of slots allocated in array
    int numItems;           // number of slots used in array

    void expand();
    int findInsertPlace(DataPoint elem) const;

    /*
     * This function walks over the contents of the interal array
     * and throws a error if any of the elements are not in decreasing
     * sorted order by priority.
     *
     * If the elements are in correctly sorted order, the function
     * returns nothing and does not throw an error.
     */
    void validateInternalState();

    /* Weird C++isms: C++ loves to make copies of things, which is usually a good thing but
     * for the purposes of this assignment requires some C++ knowledge we haven't yet covered.
     * This next line disables all copy functions to make sure you don't accidentally end up
     * debugging something that isn't your fault.
     *
     * Curious what this does? Take CS106L!
     */
    DISALLOW_COPYING_OF(PQSortedArray);
};
