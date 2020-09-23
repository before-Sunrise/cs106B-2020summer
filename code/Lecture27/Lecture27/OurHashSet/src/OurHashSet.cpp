// OurHashSet.cpp
// Implementation of OurHashSet class

#include "error.h"
#include "strlib.h"
#include "OurHashSet.h"
#include "testing/SimpleTest.h"

const int INITIAL_CAPACITY = 10;

// Initialize our member variables in the constructor
OurHashSet::OurHashSet() {
    capacity = INITIAL_CAPACITY;
    mysize = 0;
    elements = new HashNode*[capacity](); // all are initialized to nullptr using ()
}

// destructor
OurHashSet::~OurHashSet(){
    clear();
    delete [] elements;
}

// Remove all elements in our set so all buckets in our array are nullptr
void OurHashSet::clear() {
    for (int i = 0; i < capacity; i++) {
        // free list in bucket i
        while (elements[i] != nullptr) {
            HashNode* curListNode = elements[i];
            elements[i] = elements[i]->next;
            delete curListNode;
        }
    }
    mysize = 0;
}

// Check if a value is inside our set
bool OurHashSet::contains(int value) const {
    HashNode* curr = elements[getIndexOf(value)];
    while (curr != nullptr) {
        if (curr->data == value) {
            return true;
        }
        curr = curr->next;
    }
    return false;
}

// Add a given value to our set
void OurHashSet::add(int value) {
    if (!contains(value)) {
        int bucket = getIndexOf(value);
        // insert at the front of the list in that bucket
        elements[bucket] = new HashNode(value, elements[bucket]);
        mysize++;
    }
    if (mysize / capacity >= 2) {
        rehash();
    }
}

// Private helper for resizing our underlying array and rehashing existing
// elements
void OurHashSet::rehash() {
    HashNode** oldElements = elements;
    int oldCapacity = capacity;
    capacity *= 2;
    elements = new HashNode*[capacity]();
    for (int i = 0; i < oldCapacity; i++) {
        HashNode* curr = oldElements[i];
        while (curr != nullptr) {     // iterate over old bucket
            HashNode* prev = curr;
            curr = curr->next;        // don’t lose access to rest of old bucket
            int newBucket = getIndexOf(prev->data);
            prev->next = elements[newBucket]; // put prev node at front of new bucket
            elements[newBucket] = prev;       // update new bucket pointer
        }
    }
    delete[] oldElements;
}

// Returns the number of elements in our HashSet
int OurHashSet::size() const {
    return mysize;
}

// Returns true if our HashSet has no elements
bool OurHashSet::isEmpty() const {
    for (int i = 0; i < capacity; i++) {
        if (elements[i] != nullptr) {
            return false;
        }
    }
    return true;
}

// Private helper for calculating the bucket of a given a value
// Uses a built-in C++ hash function
int OurHashSet::getIndexOf(int value) const {
    return std::hash<std::int64_t>{}(value) % capacity;
}


PROVIDED_TEST("Test OurHashSet constructor and destructor."){
    OurHashSet ss;
    EXPECT(ss.isEmpty());
    EXPECT_EQUAL(ss.size(), 0);
}

PROVIDED_TEST("Test OurHashSet methods."){
    OurHashSet ss;

    /* Test add function to add some elements to the set. */
    ss.add(2000);
    EXPECT(!ss.isEmpty());
    EXPECT_EQUAL(ss.size(), 1);

    ss.add(25);
    EXPECT_EQUAL(ss.size(), 2);

    Vector<int> nums = {1, 2, 3, 4, 5, 6, 7, 8, 25};
    for (int num: nums){
        ss.add(num);
    }
    EXPECT_EQUAL(ss.size(), 10);

    /* Test contains function to see if add operations were successful. */
    EXPECT(ss.contains(1));
    EXPECT(ss.contains(2000));
    EXPECT(ss.contains(3));
    EXPECT(ss.contains(5));

    EXPECT(!ss.contains(244));
    EXPECT(!ss.contains(11));

    // Test resize
    nums = {98, 97, 196, 195, 82, 61, 27, 100, 108, 237, 125};
    for (int num: nums){
        ss.add(num);
    }
    EXPECT(ss.contains(1));
    EXPECT(ss.contains(125));
    EXPECT_EQUAL(ss.size(), 21);

    ss.clear();
    EXPECT_EQUAL(ss.size(), 0);
}

