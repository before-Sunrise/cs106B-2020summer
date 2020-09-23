// OurHashSet.h
// Header file for our HashSet of ints Class

#pragma once

#include<ostream>
using namespace std;

struct HashNode {
    int data;
    HashNode* next;

    HashNode(int value, HashNode* ptr) {
        data = value;
        next = ptr;
    }
};

class OurHashSet {
public:
    OurHashSet();
    ~OurHashSet();

    void add(int value);
    void clear();
    bool contains(int value) const;
    int size() const;
    bool isEmpty() const;

private:
    HashNode** elements;     // an array of HashNode* (an array of pointers!)
    int mysize;
    int capacity;

    // Private member functions
    int getIndexOf(int value) const;
    void rehash();
};
