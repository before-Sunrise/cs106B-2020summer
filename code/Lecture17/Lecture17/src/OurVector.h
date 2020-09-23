// OurVector.h
// Header file for our Vector of ints Class

#pragma once

#include<ostream>
using namespace std;

class OurVector {
public:
    // constructor
    OurVector();

    // destructor
    ~OurVector();

    // append value at end
    void add(int value);

    // insert a value at index
    void insert(int index, int value);

    // return value at index
    int get(int index);

    // remove value at index
    void remove(int index);

    // return the number of elements in the array
    int size();

    // true if zero elements
    bool isEmpty();

    /* Useful for viewing and debugging internal state of vector. */
    void printDebugInfo();

private:
    int* elements;          // The actual array of elements.
    int allocatedCapacity;  // How much space is allocated in our vector.
    int numItems;           // How many elements are actually in our vector.
  
    void expand();
};
