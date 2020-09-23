#include <iostream>
#include "console.h"
#include "testing/SimpleTest.h"
#include "perfect.h"
#include "soundex.h"
using namespace std;


int main() 
{
    if (runSimpleTests(NO_TESTS)) {//q2时edit from NO_TESTS to SELECTED_TESTS, part2时 edit from SELECTED_TESTS to NO_TESTS
        return 0;
    }

    //findPerfects(40000);
    // Comment out the above line and uncomment below line 
    // to switch between running perfect.cpp and soundex.cpp
    soundexSearch("res/surnames.txt");

    return 0;
}


// Do not remove or edit this test. It is here to to confirm that your code
// conforms to the expected function prototypes needed for grading
PROVIDED_TEST("Confirm function prototypes") {
    bool execute = false;
    if (execute) {
        divisorSum(0);
        isPerfect(0);
        findPerfects(0);

        smarterSum(0);
        isPerfectSmarter(0);
        findPerfectsSmarter(0);

        removeNonLetters("");
        soundex("");
        soundexSearch("");
    }
}
