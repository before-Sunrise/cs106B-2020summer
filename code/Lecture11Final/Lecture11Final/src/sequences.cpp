#include <iostream>
#include "console.h"
using namespace std;

void generateSequences(int length);
void generateSequencesHelper(int flipsLeft, string sofar);

void runSequencesDemo() {
    generateSequences(4);
}

void generateSequences(int length) {
    generateSequencesHelper(length, "");
}

void generateSequencesHelper(int flipsLeft, string sofar) {
    // Base case: no more flips left
    if (flipsLeft == 0) {
        cout << sofar << endl;
    } else {    // Recursive case: either add a "H" or "T"
        generateSequencesHelper(flipsLeft - 1, sofar + "H");
        generateSequencesHelper(flipsLeft - 1, sofar + "T");
    }
}

